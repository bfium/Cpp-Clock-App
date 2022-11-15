
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
#include"app.h"

namespace abstract
{
    namespace data
    {
        namespace command
        {
            std::unique_ptr<Command> CommandRepository::get(const std::string &) const
            {
                return nullptr;
            }

        } // namespace command

        namespace shape
        {
        } // namespace shape

    } // namespace data_abstraction

    namespace boundary
    {
        namespace proxy
        {
        } // namespace proxy

        namespace user_interaction
        {
        } // namespace user_interaction

        namespace io_device
        {

        } // namespace io_device
    }     // namespace boundary

    namespace control
    {
        namespace coordinator
        {
        } // namespace coordinator

        namespace state_dependent_control
        {
        } // namespace state_dependent_control

        namespace timer
        {
        } // namespace timer
    }     // namespace control

    namespace logic
    {
        namespace algorithm
        {
        } // namespace algorithm

        namespace business
        {
        } // namespace business

        namespace service
        {

        } // namespace service
    }     // namespace logic
} // namespace abstract

namespace service_system
{
    namespace publisher
    {
        class Publisher::PublisherImpl{
            public:
                void subscribe(const std::string &eventName, std::unique_ptr<abstract::boundary::proxy::Observer> observer);
                std::unique_ptr<abstract::boundary::proxy::Observer> unsubscribe(const std::string &eventName, const std::string &observerName);
                virtual std::unique_ptr<abstract::data::OutputData> transform(std::shared_ptr<abstract::data::InputData> i) noexcept;
                void notify(const std::string &eventName, std::shared_ptr<abstract::data::InputData>) const;
                void registerEvent(const std::string &eventName);

            private:
        };

        const std::string Publisher::name = "publisher";

        void Publisher::notify(const std::string &eventName, std::shared_ptr<abstract::data::InputData>) const
        {
            //to do
        }
        void Publisher::subscribe(const std::string &e, std::unique_ptr<abstract::boundary::proxy::Observer> o){
            // to do
        }
        std::unique_ptr<abstract::boundary::proxy::Observer> Publisher::unsubscribe(const std::string &event, const std::string &oName) noexcept{
            // to do
            return nullptr;
        }
        void Publisher::registerEvent(const std::string &){
            // to do
        }

    } // namespace Publisher

    namespace tokenizer
    {
        Tokenizer::Tokenizer(std::string){

        }
        void Tokenizer::tokenize(std::istream& is) noexcept
        {

        }
    } // namespace tokenizer

} // namespace service_system

namespace app
{
    namespace data
    {
        namespace command
        {
            void RotateCommand::executeImpl(){

            }
        } // namespace command
    }// namespace data;

    namespace client
    {
        namespace data
        {
            std::unique_ptr<abstract::data::command::Command> RotateCommandFactory::create(std::unique_ptr< abstract::data::InputData> d) const
            {
                //auto dat = dynamic_cast<RotateInputData*>(std::move(d));
                //return std::make_unique<app::data::command::RotateCommand>(new app::data::command::RotateCommand(dat->m_angle));
                return nullptr;
            }
        } // namespace data

        namespace view
        {
            namespace user_interaction
            {
                const std::string UserInterface::inputEntered = "inputEntered";

                namespace cli
                {
                    void Cli::notify(){

                    }
                    void Cli::sendInput(const char *){

                    }
                    void Cli::sendInput(std::unique_ptr<abstract::data::InputData>){

                    }
                    void Cli::run(){

                    }
                
                } // namespace cli

                namespace gui
                {
                } // namespace gui
            }     // namespace user_interaction
        }         // namespace view

        namespace controller
        {
            namespace state_dependent_control
            {
                void ClockDispatcher::dispatch(const char *cmd, const char *sender) noexcept {

                }

            } // namespace state_dependent_control
        }     // namespace controller

        namespace view
        {
            namespace boundary
            {
                namespace proxy
                {

                } // namespace proxy
            }     // namespace boundary
        }         // namespace  view
    }             // namespace client

    namespace server
    {
        namespace data
        {
            namespace shape
            {

            } // namespace shape
        } // namespace data

        namespace logic
        {
            namespace business
            {
                void ClockManager::manage(std::shared_ptr<abstract::data::command::Command> c) noexcept{

                }
                void ClockManager::undo(){

                }
                void ClockManager::redo(){

                }
            } // namespace business

        } // namespace service

        namespace boundary
        {
            namespace proxy
            {
                void ModelObserver::notifyImpl(std::shared_ptr<abstract::data::InputData> d)
                {
                }
            } // namespace proxy

        } // namespace boundary

    } // namespace server

    void Facade::run(){

    }
}
