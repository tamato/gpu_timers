#ifndef CHECKEXTENSIONS_H_
#define CHECKEXTENSIONS_H_
#include <string>
#include <cstring>
#include <glad/glad.h>

namespace ogle
{
    std::vector<bool> extensions_supported(const std::vector<std::string>& exts);

    #ifdef CHECKEXTENSIONS_IMPLEMENTATION
    std::vector<bool> extensions_supported(const std::vector<std::string>& exts)
    {
        // check for specific GL version to know which style of extension checking to do
        //std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;

        std::vector<std::string> check_extensions = exts;
        std::vector<bool> supported(exts.size(), false);

        GLint n, i, j;
        glGetIntegerv(GL_NUM_EXTENSIONS, &n);
        for (i = 0; i < n; i++) {
            const char* ext = (const char*)glGetStringi(GL_EXTENSIONS, i);

            for (j=0; j<(GLint)check_extensions.size(); ++j){
                const char* found = strstr(ext, check_extensions[j].c_str());
                if (0 != found){
                    supported[j] = true;
                    check_extensions.erase(check_extensions.begin()+j);
                    break;
                }
            }

            if (check_extensions.empty())
                break;
        }

        return (supported);
    }

    #endif //CHECKEXTENSIONS_IMPLEMENTATION
}

#endif //CHECKEXTENSIONS_H_