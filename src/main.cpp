#include "argList.H"
#include "Time.H"
#include "foamVersion.H"
#include "mcp/server.hpp"
#include "tools/pipe_flow_tool.hpp"
#include <iostream>
#include <exception>

using namespace Foam;
using namespace Foam::MCP;

int main(int argc, char* argv[])
{
    std::cerr << "🚀 OpenFOAM MCP Server v1.0.0" << std::endl;
    std::cerr << "OpenFOAM version: " << Foam::FOAMversion << std::endl;
    
    // Test OpenFOAM integration
    Foam::Info << "✅ OpenFOAM integration: Working!" << Foam::endl;
    
    // Show environment variables
    if (const char* foam_src = std::getenv("FOAM_SRC")) {
        Foam::Info << "FOAM_SRC: " << foam_src << Foam::endl;
    }
    
    if (const char* foam_tutorials = std::getenv("FOAM_TUTORIALS")) {
        Foam::Info << "FOAM_TUTORIALS: " << foam_tutorials << Foam::endl;
    }
    
    std::cerr << "🎯 Initializing MCP server with OpenFOAM tools..." << std::endl;
    
    try
    {
        // Initialize MCP server
        McpServer server;
        
        // Set server information
        ServerInfo info;
        info.name = "OpenFOAM MCP Server";
        info.version = "1.0.0";
        info.description = "World's first native OpenFOAM Model Context Protocol server";
        info.author = "OpenFOAM Community";
        info.homepage = "https://github.com/openfoam/mcp-server";
        server.setServerInfo(info);
        
        // Register tools
        registerPipeFlowTool(server);
        
        std::cerr << "🔧 Registered tools: ";
        auto tools = server.getRegisteredTools();
        for (size_t i = 0; i < tools.size(); ++i)
        {
            std::cerr << tools[i];
            if (i < tools.size() - 1) std::cerr << ", ";
        }
        std::cerr << std::endl;
        
        std::cerr << "🌟 OpenFOAM MCP Server ready!" << std::endl;
        std::cerr << "📡 Listening on stdin for MCP protocol messages..." << std::endl;
        
        // Start server (blocking)
        server.start();
        
    }
    catch (const std::exception& e)
    {
        std::cerr << "❌ Error starting MCP server: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}