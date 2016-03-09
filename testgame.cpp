#include <string>
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[]) {
    // Invoke the Java program with the passed arguments.
    string cmd = "java -cp OthelloFramework.jar TestGame";    
    argv++;
    while (--argc) {
        cmd += " ";
        cmd += *(argv++);
    }
    int toRemoveWarningMsg = system(cmd.c_str());
    toRemoveWarningMsg += 1;
    return 0;
}
