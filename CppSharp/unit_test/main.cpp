#include "stdafx.h"

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "CppSharpBridge.h"

int main(int argc, char* argv[]) {
    printf("Running Tests\n");
    CS_Init();

    int result = Catch::Session().run(argc, argv);

    return (result < 0xff ? result : 0xff);
}