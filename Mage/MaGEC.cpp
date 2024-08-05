#include "MaGEBasic.h"
#include "MaGEC.h"

#include <cstring>
#include <fstream>

bool MaGE::dumpIS(MaGEIR& instructions, const std::string &filename, const CFormat format) {
    MaGEInstruction *in = instructions.unpack();

    if (! in) { return false; }

    // open f - a.mof
    std::ofstream f("a.mof", std::ios::out | std::ios::binary);

    while (in) {
        // in-GetInstruction () for the instruction number
        std::cout << "[MaGE] converting instruction " << in->getInstruction() << std::endl;
        
        const uint32_t n = in->getInstruction();

        char conv[format];

        memcpy(conv, &(n), format);

        instructions.next();
        in = instructions.unpack();
    }
    return true;
}
