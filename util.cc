#define UNW_LOCAL_ONLY
#include <cxxabi.h>
#include <libunwind.h>
#include "util.hh"

std::map<uint64_t, std::string> g_symbols;

// Call this function to get a backtrace.
void backtrace(uint64_t *pc_val) {

    char sym[256];
    char aux[256];

    unw_cursor_t cursor;
    unw_context_t context;

    // Initialize cursor to current frame for local unwinding.
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    // Unwind frames until we get to the caller
    unw_step(&cursor);
    unw_step(&cursor);
    unw_step(&cursor);

    //while (unw_step(&cursor) > 0) {
    unw_word_t offset, pc;
    unw_get_reg(&cursor, UNW_REG_IP, &pc);
    *pc_val = pc;
    if(g_symbols.find(pc)==g_symbols.end()){
        int status; 
        if (unw_get_proc_name(&cursor, aux, sizeof(aux), &offset) == 0) {
            char* demangled = abi::__cxa_demangle(sym, nullptr, nullptr, &status);
            sprintf(sym,"(%s+0x%lx)", demangled, offset);
            std::free(demangled);
        } else {
            sprintf(sym,"(UNKNOWN+0x%lx)", offset);
        }
        g_symbols[*pc_val] = sym;
    }
}


// Creates a dict file for the PC and the calls name
void dump_symbols(){

    //Prepare all the data in the symbols
    //We will need to receive the symbols from other ranks too
    //ALL GATHER

}
