#ifndef DEF_APPLICATION_H
#define DEF_APPLICATION_H

/**
   Application bothers with the LUA registration process, state management, initial window sandbox creation.
 */
 //TODO: At some point, perform LUA isolation to prevent a random lua script from crashing the program.

class Application
{
    public:
        /** Default destructor */
        virtual ~Application();
        static Application* instance();
        int exec(int argc, char* argv[]);
        sf::Window* canvas();
        lua_State* luaState();
    protected:
        lua_State* _state;
        static Application* _instance;
        sf::Window* window;
        std::map<int, const char*> _eventStringMap;
    private:
        /** Default constructor */
        Application();
        /** Allows the various canvas events to be caught from LUA code. */
        void mapEventTable();

};

#endif // DEF_APPLICATION_H
