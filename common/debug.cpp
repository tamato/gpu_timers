#include "debug.h"

#include <iostream>

#include <glad/glad.h>

using namespace std;
using namespace ogle;

Debug::State* Debug::Instance = 0;
Debug::State* Debug::instance()
{
    if (!InstanceDestroyed && !Instance)
    {
        Instance = new State;
        Instance->init();
    }
    return Instance;
}

bool Debug::InstanceDestroyed = false;

void debugOutput(
    unsigned int source,
    unsigned int type,
    unsigned int id,
    unsigned int severity,
    int length,
    const char* message,
    void* userParam)
{
    if (GL_DEBUG_SEVERITY_NOTIFICATION == severity) // ignore messages that are not actual errors.
        return;

    cout << "OGL Debugger Error: \n"
         << "\tSource: "    << Debug::sourceError(source) << "\n"
         << "\tType: "      << Debug::typeError(type) << "\n"
         << "\tID: <"       << id << "> " << Debug::idError(id) << "\n"
         << "\tSeverity: "  << Debug::severityError(severity) << "\n"
         << "\tMessage: "   << message
         << endl;
    exit(1);
}

void Debug::init()
{
    if (State *i = instance())
    {
        i->init();
    }
}

const string NotKnown = "ID for error not known";
const string& Debug::sourceError(unsigned int id)
{
    if (State *i = instance())
    {
        return i->Source[id];
    }
    return NotKnown;
}

const string& Debug::typeError(unsigned int id)
{
    if (State *i = instance())
    {
        return i->Type[id];
    }
    return NotKnown;
}

const string& Debug::idError(unsigned int id)
{
    if (State *i = instance())
    {
        return i->IDs[id];
    }
    return NotKnown;
}

const string& Debug::severityError(unsigned int id)
{
    if (State *i = instance())
    {
        return i->Severity[id];
    }
    return NotKnown;
}

void Debug::shutdown()
{
    delete Instance;
    Instance = nullptr;
    InstanceDestroyed = true;
}

void Debug::State::init()
{
    Source[GL_DEBUG_SOURCE_API_ARB] = "GL_DEBUG_SOURCE_API_ARB";
    Source[GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB] = "GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB";
    Source[GL_DEBUG_SOURCE_SHADER_COMPILER_ARB] = "GL_DEBUG_SOURCE_SHADER_COMPILER_ARB";
    Source[GL_DEBUG_SOURCE_THIRD_PARTY_ARB] = "GL_DEBUG_SOURCE_THIRD_PARTY_ARB";
    Source[GL_DEBUG_SOURCE_APPLICATION_ARB] = "GL_DEBUG_SOURCE_APPLICATION_ARB";
    Source[GL_DEBUG_SOURCE_OTHER_ARB] = "GL_DEBUG_SOURCE_OTHER_ARB";

    Type[GL_DEBUG_TYPE_ERROR_ARB] = "GL_DEBUG_TYPE_ERROR_ARB";
    Type[GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB] = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB";
    Type[GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB] = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB";
    Type[GL_DEBUG_TYPE_PORTABILITY_ARB] = "GL_DEBUG_TYPE_PORTABILITY_ARB";
    Type[GL_DEBUG_TYPE_PERFORMANCE_ARB] = "GL_DEBUG_TYPE_PERFORMANCE_ARB";
    Type[GL_DEBUG_TYPE_OTHER_ARB] = "GL_DEBUG_TYPE_OTHER_ARB";
    
    Type[GL_DEBUG_TYPE_MARKER] = "GL_DEBUG_TYPE_MARKER";
    Type[GL_DEBUG_TYPE_PUSH_GROUP] = "GL_DEBUG_TYPE_PUSH_GROUP";
    Type[GL_DEBUG_TYPE_POP_GROUP] = "GL_DEBUG_TYPE_POP_GROUP";

    Severity[GL_DEBUG_SEVERITY_HIGH_ARB] = "GL_DEBUG_SEVERITY_HIGH_ARB";            // Any GL error; dangerous undefined behavior; any GLSL or ARB shader compiler andlinker errors;
    Severity[GL_DEBUG_SEVERITY_MEDIUM_ARB] = "GL_DEBUG_SEVERITY_MEDIUM_ARB";        // Severe performance warnings; GLSLor other shader compiler and linker warnings; use of currently deprecated behavior
    Severity[GL_DEBUG_SEVERITY_LOW_ARB] = "GL_DEBUG_SEVERITY_LOW_ARB";              // Performance warnings from redundant state changes; trivial undefined behavior
    Severity[GL_DEBUG_SEVERITY_NOTIFICATION] = "GL_DEBUG_SEVERITY_NOTIFICATION";    // Any message which is not an error or performance concern

    /** General OpenGL Errors */
    IDs[GL_INVALID_ENUM] = "GL_INVALID_ENUM";
    IDs[GL_INVALID_VALUE] = "GL_INVALID_VALUE";
    IDs[GL_INVALID_OPERATION] = "GL_INVALID_OPERATION";
    IDs[GL_STACK_OVERFLOW] = "GL_STACK_OVERFLOW";
    IDs[GL_STACK_UNDERFLOW] = "GL_STACK_UNDERFLOW";
    IDs[GL_OUT_OF_MEMORY] = "GL_OUT_OF_MEMORY";

    /** Framebuffer errors */

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    glDebugMessageCallbackARB((GLDEBUGPROCARB)debugOutput, nullptr);
}

Debug::Debug()
{

}

Debug::~Debug()
{

}

Debug::Debug(const Debug& other)
{

}

Debug& Debug::operator=(const Debug& other)
{
    return *this;
}
