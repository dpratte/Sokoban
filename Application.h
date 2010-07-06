#ifndef DEF_APPLICATION_H
#define DEF_APPLICATION_H


class Application
{
    public:
        /** Default destructor */
        virtual ~Application();
        static Application* instance();
        int exec(int argc, char* argv[]);
    protected:
        lua_State* _state;
        static Application* _instance;
    private:
        /** Default constructor */
        Application();
};

#endif // DEF_APPLICATION_H
