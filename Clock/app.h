/*
    Copyright (C) 2022  Barth.Feudong
    Author can be contacted here: <https://github.com/mrSchaffman/Cpp-Clock-App>

    This file is part of the Clock project. using the Win32 API and The COM

    Clock is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Clock is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/
#include<string>
#include<stack>
#include<vector>
#include<iosfwd>
#include<memory>
#include<map>
#include<unordered_map>

// #include <Windows.h>
// #include <d2d1.h>
// #pragma comment(lib, "d2d1")

namespace external
{
    namespace user
    {
        
    } // namespace user

    namespace device
    {
        namespace input_device
        {
            
        } // namespace input_device

        namespace output_device
        {
            
        } // namespace output_device
        namespace input_output_device
        {
            
        } // namespace input_output_device
    }     // namespace device

    namespace system
    {
        // RAS
    } // namespace system
    
} // namespace external

namespace abstract
{
    namespace data
    {
        class Data
        {
        public:
            virtual ~Data() = default;
        };

        class InputData : public Data
        {
        public:
            virtual~InputData() = default;
        };

        class OutputData : public Data
        {
        public:
            virtual~OutputData() = default;
        };

        namespace command
        {
            class Command
            {
            public:
                virtual ~Command() = default;
                void execute() noexcept {
                    preConditionCheck();
                    executeImpl();
                    postConditionCheck();
                }

            protected:
                Command() = default;
                Command(const Command &) = default;
            protected:
                virtual void executeImpl() = 0;
                virtual void preConditionCheck(){};
                virtual void postConditionCheck(){};

            private:
                Command(Command &&) = delete;
                Command &operator=(const Command &) = delete;
                Command &operator=(Command &&) = delete;
            };
            // inline void release(Command*c){
            //     if(c)
            //         delete c;
            // }
            // using unique_command_ptr = std::unique_ptr<Command, dcltype(&release)>;

            class CommandFactory
            {
            public:
                virtual~CommandFactory() {}
                virtual std::unique_ptr<Command> create(std::unique_ptr<abstract::data::InputData>) const = 0;
            };

            class CommandRepository
            {
            private:
                friend class Command;

            public:
                ~CommandRepository() {}
                std::unique_ptr<Command> get(const std::string &) const;

            private:
                std::map<std::string, std::unique_ptr<Command>> m_commands;
            };
        } // namespace command

        namespace shape
        {
            class Shape
            {
            public:
                virtual~Shape() {}
            };
        } // namespace shape
        
    } // namespace data_abstraction

    namespace boundary
    {
        namespace proxy
        {            
            class Observer
            {
            public:
                Observer(const std::string& n):m_name{n} {}
                virtual~Observer() {}
                void notify(std::shared_ptr<abstract::data::InputData> d) { notifyImpl(d); }

            protected:
                virtual void notifyImpl(std::shared_ptr < abstract::data::InputData> d) = 0;

            private:
                std::string m_name;
            };
        } // namespace proxy

        namespace user_interaction
        {
            class UserInteraction
            {                
            public:
                virtual ~UserInteraction() = default;
                virtual void notify() {};       // if MVC
                virtual void sendInput(const char *) = 0;
                virtual void sendInput(std::unique_ptr<abstract::data::InputData>) = 0;
            };
        } // namespace user_interaction
        
        namespace io_device
        {
            
        } // namespace io_device   
    } // namespace boundary
    
    namespace control
    {
        namespace coordinator
        {
            class Coordinator
            {
            public:
                virtual~Coordinator() {}
            };
        } // namespace coordinator

        namespace state_dependent_control
        {
            class Disptacher
            {
            public:
                virtual~Disptacher() {}
                virtual void dispatch(const char *cmd, const char *sender) noexcept = 0;
            };
        } // namespace state_dependent_control
        
        namespace timer
        {
            class Timer
            {
            public:
                virtual~Timer() {}
            };
        } // namespace timer      
    } // namespace control

    namespace logic
    {
        namespace algorithm
        {
            class Strategy
            {
            public:
                virtual~Strategy() {}
            };
        } // namespace algorithm

        namespace business
        {
            class Manager
            {
            private:
                
            public:
                virtual~Manager() {}
                virtual void manage(std::shared_ptr<abstract::data::command::Command> c)noexcept = 0;
            };
        } // namespace business
        
        namespace service
        {
            class Service
            {
            public:
                virtual ~Service() = default;
                virtual const std::string &getName() const = 0;
                virtual const std::string &getLocation() const = 0;
                virtual const std::string &getDescription() const = 0;
                virtual std::unique_ptr<abstract::data::OutputData> transform(std::shared_ptr<abstract::data::InputData> i) noexcept{};                
            };

        } // namespace service
    } // namespace logic   
} // namespace abstract

namespace service_system
{
    namespace publisher
    {
        enum EventType
        {
            DEFAULT,
        };
        class Publisher : public abstract::logic::service::Service
        {
            static const std::string name;

            using o_list = std::unordered_map<std::string, abstract::boundary::proxy::Observer>;
            using event_o_list = std::unordered_map<std::string, o_list>;

        public:
            const std::string &getName() const override;
            const std::string &getLocation() const override;
            const std::string &getDescription() const override;

        public:
            virtual~Publisher() {}
            void notify(const std::string &e, std::shared_ptr<abstract::data::InputData> d);
            //void notify(EventType e, std::shared_ptr<abstract::data::InputData> d);
            void subscribe(const std::string &e, std::unique_ptr<abstract::boundary::proxy::Observer> o);
            std::unique_ptr<abstract::boundary::proxy::Observer> unsubscribe(const std::string &event, const std::string &oName) noexcept;
        protected:
            void registerEvent(const std::string &);

        private:
            event_o_list m_observers;
        };
    } // namespace Publisher

    namespace tokenizer
    {
        class Tokenizer : public abstract::logic::service::Service
        {
            using const_iterator = std::vector<std::string>::const_iterator;
            static const std::string name;

        public:
            const std::string &getName() const override;
            const std::string &getLocation() const override;
            const std::string &getDescription() const override;

        public:
            Tokenizer(std::string);
            // Tokenizer(std::istream);
            ~Tokenizer() {}
            const_iterator begin() const { return m_tokens.cbegin(); }
            const_iterator end() const { return m_tokens.cend(); }
            size_t size() const { return m_tokens.size(); }

        private:
            void tokenize(std::istream& is) noexcept;
        private:
            std::vector<std::string> m_tokens;
        };
    } // namespace tokenizer
    
} // namespace service_system

namespace app
{
    namespace data
    {
        namespace command
        {
            class RotateCommand : public abstract::data::command::Command
            {
            public:
                RotateCommand(double angle):m_angle{angle}{}
                ~RotateCommand();
                void executeImpl() override;

            private:
                double m_angle;
            };
        } // namespace command
    }     // namespace data;
    
    namespace client
    {
        namespace data
        {
            struct RotateInputData : public abstract::data::InputData
            {
                double m_angle;
            };

            class RotateCommandFactory: public abstract::data::command::CommandFactory
            {                
            public:
                std::unique_ptr<abstract::data::command::Command> create(std::unique_ptr< abstract::data::InputData >) const override;
                ~RotateCommandFactory() {}
            };
        } // namespace data

        namespace view
        {
            namespace user_interaction
            {
                class UserInterface : protected service_system::publisher::Publisher,public abstract::boundary::user_interaction::UserInteraction
                {
                    static const std::string inputEntered;

                public:
                    virtual ~UserInterface() {}

                public:
                    using Publisher::notify;
                    using Publisher::subscribe;
                    using Publisher::unsubscribe;

                public:
                    // virtual void sendInput(const char *) = 0;
                    // virtual void sendInput(std::unique_ptr<abstract::data::InputData>) = 0;
                };

                namespace cli
                {
                    class Cli: public UserInterface
                    {
                    public:
                        Cli(std::istream&is,std::ostream&os) :m_is{is},m_os{os}{}
                        ~Cli() {}
                        void notify() override; 
                        void sendInput(const char *) override;
                        void sendInput(std::unique_ptr<abstract::data::InputData>) override;
                        void run();

                    private:
                        std::istream& m_is;
                        std::ostream& m_os;
                    };
                } // namespace cli
                
                namespace gui
                {
                    // template <class TYPE>
                    // class BaseWindow
                    // {
                    // public:
                    //     // The Ctor of the Base Class.
                    //     BaseWindow() : m_hwnd(NULL) {}
                    //     static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
                    //     {
                    //         TYPE *pThis = NULL;
                    //         if (uMsg == WM_NCCREATE)
                    //         {
                    //             CREATESTRUCT *pCreate = (CREATESTRUCT *)lParam;
                    //             pThis = (TYPE *)pCreate->lpCreateParams;
                    //             SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
                    //             pThis->m_hwnd = hwnd;
                    //         }
                    //         else
                    //         {
                    //             pThis = (TYPE *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
                    //         }
                    //         if (pThis)
                    //         {
                    //             return pThis->HandleMessage(uMsg, wParam, lParam);
                    //         }
                    //         else
                    //         {
                    //             return DefWindowProc(hwnd, uMsg, wParam, lParam);
                    //         }
                    //     }

                    //     BOOL Create(
                    //         PCWSTR lpWindowName,
                    //         DWORD dwStyle,
                    //         DWORD dwExStyle = 0,
                    //         int x = CW_USEDEFAULT,
                    //         int y = CW_USEDEFAULT,
                    //         int nWidth = CW_USEDEFAULT,
                    //         int nHeight = CW_USEDEFAULT,
                    //         HWND hWndParent = 0,
                    //         HMENU hMenu = 0)
                    //     {
                    //         WNDCLASS wc = {0};

                    //         wc.lpfnWndProc = TYPE::WindowProc;
                    //         wc.hInstance = GetModuleHandle(NULL);
                    //         wc.lpszClassName = ClassName();
                    //         RegisterClass(&wc);
                    //         m_hwnd = CreateWindowEx(
                    //             dwExStyle,
                    //             ClassName(),
                    //             lpWindowName,
                    //             dwStyle,
                    //             x,
                    //             y,
                    //             nWidth,
                    //             nHeight,
                    //             hWndParent,
                    //             hMenu,
                    //             GetModuleHandle(NULL),
                    //             this);

                    //         return (m_hwnd ? TRUE : FALSE);
                    //     }

                    //     HWND Window() const { return m_hwnd; }

                    // protected:
                    //     virtual PCWSTR ClassName() const = 0;
                    //     virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

                    //     HWND m_hwnd;
                    // };
                    // template <class T>
                    // void SafeRelease(T **ppT)
                    // {
                    //     if (*ppT)
                    //     {
                    //         (*ppT)->Release();
                    //         *ppT = NULL;
                    //     }
                    // }
                    // class MainWindow : public BaseWindow<MainWindow>
                    // {
                    // public:
                    //     MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL) {}

                    //     PCWSTR ClassName() const { return L"Sample Window Class"; }
                    //     LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

                    // private:
                    //     // COM interfaces...
                    //     ID2D1Factory *pFactory;
                    //     ID2D1HwndRenderTarget *pRenderTarget;
                    //     ID2D1SolidColorBrush *pBrush;
                    //     ID2D1SolidColorBrush *pBrushHand;
                    //     D2D1_ELLIPSE ellipse;

                    //     // Recalculate Drawing layout when the size of the Window changes.
                    //     void DrawClockHand(float fHandLength, float fAngle, float fStrockeWidth);
                    //     void RenderClock();

                    //     // Create the Resource needed for Drawing
                    //     HRESULT CreateGraphicsResource();
                    //     void DiscardGraphicsResource();

                    //     // Draw
                    //     void OnPaint();
                    //     void Resize();
                    // };

                } // namespace gui
            } // namespace user_interaction
        } // namespace view
        
        namespace controller
        {
            namespace state_dependent_control
            {
                class ClockDispatcher: public abstract::control::state_dependent_control::Disptacher
                {                    
                public:
                    ClockDispatcher(view::user_interaction::UserInterface &ui):m_ui{ui}{};
                    ~ClockDispatcher();
                    void dispatch(const char *cmd, const char *sender) noexcept override;

                private:
                    view::user_interaction::UserInterface &m_ui;
                };
            } // namespace state_dependent_control
        } // namespace controller
        
        namespace  view
        {
            namespace boundary
            {
                namespace proxy
                {
                    class ViewObserver
                    {
                    private:
                        client::controller::state_dependent_control::ClockDispatcher &m_cd;

                    public:
                        ViewObserver(client::controller::state_dependent_control::ClockDispatcher &c) : m_cd{c} {}
                        ~ViewObserver() {}
                    };
                } // namespace proxy
            } // namespace boundary
        } // namespace  view 
    } // namespace client

    namespace server
    {
        namespace data
        {
            namespace shape
            {
                class Rectangle : public abstract::data::shape::Shape
                {
                public:
                    Rectangle(double x1, double y1, double x2, double y2)
                        : m_x1{x1}, m_y1{y1}, m_x2{x2}, m_y2{y2} {}
                    ~Rectangle() {}

                private:
                    double m_x1, m_y1, m_x2, m_y2;
                };

                class Circle : public abstract::data::shape::Shape
                {
                private:
                    double m_x, m_y, m_radius;

                public:
                    Circle(double x,double y,double r):m_x(x),m_y(y),m_radius(r) {}
                    ~Circle() {}
                };
            } // namespace shape

            class Model
            {
            private:
                /* data */
            public:
                Model(/* args */) {}
                ~Model() {}
            };
        } // namespace data

        namespace logic
        {
            namespace business
            {
                class ClockManager : public abstract::logic::business::Manager
                {
                public:
                    void manage(std::shared_ptr<abstract::data::command::Command> c) noexcept override;
                    ~ClockManager() {}
                    void undo();
                    void redo();

                private:
                    std::stack < std::unique_ptr<abstract::data::command::Command>> m_undo_repository;
                    std::stack<std::unique_ptr<abstract::data::command::Command>> m_redo_repository;
                };
            } // namespace business

        } // namespace service
        
        namespace boundary  
        {
            namespace proxy
            {
                class ModelObserver : public abstract::boundary::proxy::Observer
                {
                public:
                    ModelObserver(client::view::user_interaction::UserInterface &u) : Observer("ModelObserver"), m_ui{u} {}
                    ~ModelObserver() {}

                private:
                    void notifyImpl(std::shared_ptr<abstract::data::InputData> d) override;

                private:
                    client::view::user_interaction::UserInterface &m_ui;
                };
            } // namespace proxy
            
        } // namespace boundary

    } // namespace server

    class Facade
    {
    private:
        //client::view::user_interaction::gui::MainWindow m_window;
        //server::
    public:
        Facade() {}
        ~Facade() {}
        void run();
    };

} // namespace app
