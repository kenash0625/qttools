import "../BareMetalApplication.qbs" as BareMetalApplication

BareMetalApplication {
    condition: {
        if (qbs.toolchainType === "sdcc")
            return true;
        if (qbs.toolchainType === "msvc")
            return true;
        if (qbs.toolchainType === "clang-cl")
            return true;
        if (qbs.toolchainType === "iar")
            return true;
        if (qbs.toolchainType === "keil") {
            if (qbs.architecture === "mcs51"
                || qbs.architecture === "mcs251"
                || qbs.architecture === "c166") {
                return true;
            }
            if (cpp.compilerName.startsWith("armcc")) {
                console.info("using short listing file names");
                return true;
            }
        }
        console.info("unsupported toolset: %%"
            + qbs.toolchainType + "%%, %%" + qbs.architecture + "%%");
        return false;
    }
    cpp.generateCompilerListingFiles: true
    files: ["main.c", "fun.c"]
}
