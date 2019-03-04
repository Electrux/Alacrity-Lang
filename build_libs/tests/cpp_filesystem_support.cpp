#ifdef CXX_FS_EXPERIMENTAL
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

int main() { fs::path( "." ); return 0; }