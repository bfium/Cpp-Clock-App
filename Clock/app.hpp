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
#include<vector>
#include<iosfwd>
#include<memory>
#include<map>
#include<unordered_map>

namespace external
{
    namespace user
    {
        
    } // namespace user

    namespace io_device
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
    } // namespace device

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
            virtual ~InputData() = default;
        };

        class OutputData : public Data
        {
        public:
            virtual ~OutputData() = default;
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
            // using unique_command_ptr = std::unique_ptr<Command, &release>;

            class CommandFactory
            {
            public:
                virtual~CommandFactory() {}
                virtual std::unique_ptr<Command> create() const = 0;
            };

            class CommandRepository
            {
            private:
                friend Command;
                std::map<std::string, std::unique_ptr<Command>> m_commands;

            public:
                ~CommandRepository() {}
                std::unique_ptr<Command> get(const std::string &) const;
            };
        } // namespace command
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

            private:
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
                virtual void dispatch(const char *cmd, const char *sender)noexcept=0;
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
    namespace Publisher
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
            void tokenize(std::istream is) noexcept;
        private:
            std::vector<std::string> m_tokens;
        };
    } // namespace tokenizer
    
} // namespace service_system

