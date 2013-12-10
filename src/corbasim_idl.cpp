#include <iostream>
#include <idlmm.hpp>
#include <parser/idl_parser.hpp>
#include "output_file.hpp"

using namespace corbasim::idl;

int main(int argc, char **argv)
{
    idlmm::TranslationUnit_ptr res = NULL;

    if (argc < 2)
        res = idl::parse(std::cin);
    else
        res = idl::parse(argv[1]);

    bool err = (res == NULL);

    if (err)
        std::cerr << "Error!" << std::endl;
    else
    {
        output_file adapted("adapted.hpp");

        adapted % "// Generated file";

        {
            const nested_region r(adapted);
            adapted % "// Nested region";
        }
    }

    delete res;
    return err;
}
