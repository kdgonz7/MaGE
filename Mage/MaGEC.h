#include "MaGEBasic.h"
#include <iostream>
#include <fstream>

namespace MaGE {
    enum CFormat {
        BIT8 = 1,
        BIT32 = 4,
    };
    bool dumpIS(MaGE::MaGEIR& instructions, const std::string& filename, const CFormat);
}
