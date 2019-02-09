#include <iostream>
#if __FreeBSD__
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

int main() { fs::path( "." ); return 0; }