cmd_Release/obj.target/debug.node := g++ -o Release/obj.target/debug.node -shared -pthread -rdynamic -m64  -Wl,-soname=debug.node -Wl,--start-group Release/obj.target/debug/debug.o -Wl,--end-group 
