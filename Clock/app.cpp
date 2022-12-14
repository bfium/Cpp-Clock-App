#include"app.h"
#include<algorithm>
#include<iterator>
#include<iostream>
#include<regex>

#define ADAM_CharBufferSize 256
using namespace std;

namespace service_system
{
	namespace html
	{
		namespace data
		{

		}

		namespace boundary
		{
			namespace proxy
			{

			} // namespace proxy
		}

		namespace logic
		{
			namespace business
			{

			} // namespace business

			namespace service
			{
				const std::string HtmlService::name = "html";
			}
		}
	}

	namespace publisher
	{
		const std::string Publisher::name = "publisher";

		namespace publisher_data_abstraction
		{

		}

		class Publisher::PublisherImpl
		{
		public:
			PublisherImpl() = default;
			~PublisherImpl() = default;

			void subscribe(const std::string& eventName, std::unique_ptr<abstraction::boundary::proxy::Observer> observer);
			void unsubscribe(const std::string& eventName, const std::string& observerName);
			void notify(const string& eventName, std::shared_ptr<abstraction::data::Data>) const;

			void registerEvent(const string& eventName);

		private:
			using ObserversList = std::unordered_map<string, unique_ptr<abstraction::boundary::proxy::Observer>>;
			using Observers = std::unordered_map<string, ObserversList>;

			Observers m_observers;
		};

		Publisher::Publisher()
		{
			impl = std::make_unique<PublisherImpl>();
		}
		void Publisher::subscribe(const string& eventName, unique_ptr<abstraction::boundary::proxy::Observer> observer)
		{
			impl->subscribe(eventName, std::move(observer));
		}
		void Publisher::unsubscribe(const string& eventName, const string& observerName)
		{
			return impl->unsubscribe(eventName, observerName);
		}
		Publisher::~Publisher()
		{
			// std::unique_ptr requires a definition of the destructor instead
			// of using the default because the destructor must appear in a scope
			// in which the complete definition of the template argument for
			// std::unique_ptr is known
		}
		void Publisher::notify(const string& eventName, shared_ptr<abstraction::data::Data> data) const
		{
			impl->notify(eventName, data);
		}
		void Publisher::registerEvent(const string& eventName)
		{
			impl->registerEvent(eventName);
		}

		void Publisher::PublisherImpl::subscribe(const string& eventName, std::unique_ptr<abstraction::boundary::proxy::Observer> observer)
		{
			auto ptr = m_observers.find(eventName);
			if (ptr == std::end(m_observers))
			{
				std::ostringstream oss;
				oss << "Event with name '" << eventName << "' not supported";
				throw abstraction::data::exception::Exception(oss.str());
			}
			else
			{
				auto iter = m_observers[eventName].insert(std::make_pair(observer->getName(), std::move(observer)));
				if (!iter.second)
				{
					std::ostringstream oss;
					oss << "Observer '" << observer->getName() << "' is already registered";
					throw abstraction::data::exception::Exception(oss.str());
				}
			}
		}

		void Publisher::PublisherImpl::unsubscribe(const string& eventName, const string& observerName)
		{
			auto count = m_observers.count(eventName);
			if (count)
			{
				auto found = m_observers[eventName].erase(observerName);
				if (!found)
				{
					std::ostringstream oss;
					oss << "Observer '" << observerName << "' not found registered";
					throw abstraction::data::exception::Exception(oss.str());
				}
			}
			else
			{
				std::ostringstream oss;
				oss << "Event with name '" << eventName << "' not supported";
				throw abstraction::data::exception::Exception(oss.str());
			}
		}

		void Publisher::PublisherImpl::notify(const string& eventName, shared_ptr<abstraction::data::Data> event_) const
		{
			auto ptr = m_observers.find(eventName);
			if (ptr != std::end(m_observers))
			{
				const auto& obsList = ptr->second;

				for (const auto& obs : obsList)
					obs.second->notify(event_);
			}
			else
			{
				std::ostringstream oss;
				oss << "Event with name '" << eventName << "' not supported";
				throw abstraction::data::exception::Exception(oss.str());
			}
		}

		void Publisher::PublisherImpl::registerEvent(const string& eventName)
		{
			auto i = m_observers.find(eventName);
			if (i != m_observers.end())
				throw abstraction::data::exception::Exception{ "Event already registered" };

			m_observers[eventName] = ObserversList{};
		}
	}

	namespace tokenizer
	{
		namespace logic
		{
			namespace business
			{

			} // namespace business

			namespace service
			{
				const std::string TokenizerService::name = "tokenizer";

				TokenizerService::~TokenizerService()
				{
				}

				abstraction::data::OutputData* TokenizerService::transform(std::shared_ptr<abstraction::data::InputData> d) {

					auto data = dynamic_pointer_cast<data::TokenizerInputData>(d);
					string ss = data->getData();
					char token = data->getToken();
					size_t pos{};

					while ((pos = ss.find(token)) != std::string::npos)
						ss.replace(pos, 1, " ");

					std::istringstream iss{ ss };
					vector<string>tokens_{};
					tokens_.assign(std::istream_iterator<string>{iss}, std::istream_iterator<string>{});

					for (auto& i : tokens_)
						std::transform(i.begin(), i.end(), i.begin(), ::toupper);

					return new data::TokenizerOutputData(tokens_);
				}

			}
		}
	}
}

namespace broker_system
{
	namespace white_page
	{
		abstraction::data::OutputData* BrokerForwarder::forward(const std::string& serviceName, const abstraction::data::InputData& data) const noexcept
		{
			auto ptr = std::find_if(m_services.cbegin(), m_services.cend(), [&serviceName](auto& d)
				{ return serviceName == d.first; });
			if (ptr != m_services.cend())
				return ptr->second->transform(std::make_shared<abstraction::data::InputData>(data));
			return nullptr;
		}

		std::shared_ptr<abstraction::logic::service::Service> BrokerHandler::getService(const std::string& serviceName) const noexcept
		{
			auto ptr = std::find_if(m_services.cbegin(), m_services.cend(), [&serviceName](auto& d)
				{ return serviceName == d.first; });
			if (ptr != m_services.cend())
				return (ptr->second);
			return nullptr;
		}
	}

	namespace yellow_page_broker
	{

	}
}

namespace app
{
	namespace data_abstraction
	{
		void RotateCommand::executeImpl()noexcept
		{
			using namespace app::server_subsystem::boundary::proxy;
			ModelProxy::getInstance().rotate(m_hand, m_fAngle, true);
		}
		void RotateCommand::undoImpl()noexcept
		{

		}
	}

	namespace server_subsystem
	{
		namespace data_abstraction
		{
			ModelProxyImpl::ModelProxyImpl() : m_data{}
			{
				initialize();
			}

			void ModelProxyImpl::initialize()noexcept
			{
				m_data["hoursHand"] = Rectangle(10.0f, 10.0f, 20.0f,20.0f);
				m_data["minutesHand"] = Rectangle(10.0f, 10.0f, 40.0f,20.0f);
				m_data["secondsHand"] = Rectangle(10.0f, 10.0f, 60.0f,20.0f);
			}
		}

		namespace boundary
		{
			namespace proxy
			{
				const std::string ModelProxy::resultAvailable = "result available";
				const std::string ModelProxy::adamError = "AdamError";

				void ModelObserver::notifyImpl(std::shared_ptr<abstraction::data::Data>d)
				{
					m_ui.sendOutput(d);

					return;
				}
				void ModelProxy::rotate(const string& hand, float angle, bool notify)noexcept {
					//auto rect = m_data.[hand];

				}

				ModelProxy& ModelProxy::getInstance()
				{
					static ModelProxy instance;
					return instance;
				}
				ModelProxy::ModelProxy() :/*AdamProxyImpl()*/ m_data{} {
					registerEvent(ModelProxy::resultAvailable);
					registerEvent(ModelProxy::adamError);
				}
			}
		}

		namespace control
		{
			namespace coordinator
			{
				class ServerCoordinator::ServerCoordinatorImpl
				{
				public:
					virtual ~ServerCoordinatorImpl() {}

					virtual size_t getUndoSize() const = 0;
					virtual size_t getRedoSize() const = 0;

					virtual void executeCommand(abstraction::data::command::unique_command_ptr c) = 0;
					virtual void undo() = 0;
					virtual void redo() = 0;
					virtual void refresh() = 0;
				};

				class ServerCoordinator::UndoRedoStackStrategy : public ServerCoordinator::ServerCoordinatorImpl
				{
				public:
					size_t getUndoSize() const override { return undoStack_.size(); }
					size_t getRedoSize() const override { return redoStack_.size(); }

					void executeCommand(abstraction::data::command::unique_command_ptr c) override;
					void undo() override;
					void redo() override;
					void refresh() override;

				private:
					void flushStack(stack<abstraction::data::command::unique_command_ptr>& st);

					stack<abstraction::data::command::unique_command_ptr> undoStack_;
					stack<abstraction::data::command::unique_command_ptr> redoStack_;
				};

				void ServerCoordinator::UndoRedoStackStrategy::executeCommand(abstraction::data::command::unique_command_ptr c)
				{
					c->execute();

					undoStack_.push(std::move(c));
					flushStack(redoStack_);
				}

				void ServerCoordinator::UndoRedoStackStrategy::undo()
				{
					if (getUndoSize() == 0) return;

					auto& c = undoStack_.top();
					redoStack_.push(std::move(c));
					undoStack_.pop();

					if (getUndoSize() == 0) return;
					auto& d = undoStack_.top();
					d->execute();
				}

				void ServerCoordinator::UndoRedoStackStrategy::refresh() {
					// browser_system::server_subsystem::boundary::proxy::ModelProxy::getInstance().refresh();

					//if (getUndoSize() == 0) return;
					//
					//auto& d = undoStack_.top();
					//d->execute();

				}

				void ServerCoordinator::UndoRedoStackStrategy::redo()
				{
					if (getRedoSize() == 0) return;

					auto& c = redoStack_.top();
					c->execute();

					undoStack_.push(std::move(c));
					redoStack_.pop();
				}

				void ServerCoordinator::UndoRedoStackStrategy::flushStack(stack<abstraction::data::command::unique_command_ptr>& st)
				{
					while (!st.empty())
						st.pop();
				}

				ServerCoordinator::ServerCoordinator(UndoRedoStrategy st)
				{
					switch (st)
					{
					case UndoRedoStrategy::ListStrategy:
						//pimpl_ = make_unique<UndoRedoListStrategy>();
						break;

					case UndoRedoStrategy::StackStrategy:
						pimpl_ = make_unique<UndoRedoStackStrategy>();
						break;

					case UndoRedoStrategy::ListStrategyVector:
						//pimpl_ = make_unique<UndoRedoListStrategyVector>();
						break;
					}
				}

				ServerCoordinator::~ServerCoordinator()
				{
				}
				void ServerCoordinator::executeCommand(abstraction::data::command::unique_command_ptr c)
				{
					//cout << "The Command arrived at Server Side " << endl;
					pimpl_->executeCommand(std::move(c));
				}
				size_t ServerCoordinator::getUndoSize() const
				{
					return pimpl_->getUndoSize();
				}

				size_t ServerCoordinator::getRedoSize() const
				{
					return pimpl_->getRedoSize();
				}
				void ServerCoordinator::undo()
				{
					pimpl_->undo();
				}
				void ServerCoordinator::refresh()
				{
					pimpl_->refresh();
				}
				void ServerCoordinator::redo()
				{
					pimpl_->redo();
				}

				class ServerCoordinator::UndoRedoListStrategyVector
				{
				};
				class ServerCoordinator::UndoRedoListStrategy
				{
				};
			}

			namespace state_dependent_control
			{
				// RAS
			}

			namespace timer
			{
				// RAS
			}

		}

		namespace logic
		{
			namespace business_logic
			{

			}

			namespace algorithm
			{
				// RAS
			}

			namespace service
			{

			}
		}
	}

	namespace client_subsystem
	{
		namespace view
		{
			namespace boundary
			{
				// View
				namespace user_interaction
				{
					const char* UserInterface::InputEntered = "Input entered";

					namespace cli
					{
						void CustomerInteraction::run()
						{
							vector<string> ticks{ "0.1", "0.9 ","2.7 ","30.5 " };
							//string line{ "0.1 0.9 2.7 30.5 20.9 25.7 3.5" };

							auto service = broker_system::white_page::BrokerHandler::getInstance().getService("tokenizer");

							// Build the data needed for the Service direct onto the Service
							// itself.
							// to be done in the next iterations.
							// service.setData(...);


							//using namespace service_system::tokenizer;
							//using namespace service_system::tokenizer::logic::service;
							//using namespace service_system::tokenizer::data;
							//shared_ptr<TokenizerInputData>iData(new TokenizerInputData(line, ' '));

							//auto prt = service->transform(iData);

							//unique_ptr<TokenizerOutputData> result( dynamic_cast<TokenizerOutputData*>(prt));
							
							for (auto&& t : ticks)
								notify(InputEntered, 
									make_shared<data::UserInterfaceIntputData>(
										data::UserInterfaceIntputData(t, "timer")
										)
								);
						}
						void CustomerInteraction::sendInput()
						{
						}
						void CustomerInteraction::sendOutput(const char* msg)
						{
							m_os << "CustomerInteraction::sendOutput-> Notifaction arrived at the View Side" << endl;
							m_os << msg << endl;
						}
						void CustomerInteraction::sendOutput(shared_ptr<abstraction::data::Data>d) {
							// Todo
						}
					}

					namespace gui {

						void Win::layout()
						{

						}

						void GraphicalUserInterface::sendInput()
						{
							// to do if MVC architecture
							// get model data and update the View...
						}
						void GraphicalUserInterface::sendOutput(const char* s)
						{
							// dispatche the Data the GUI Elments
						}

						void GraphicalUserInterface::sendOutput(shared_ptr<abstraction::data::Data>d) {
							// dispatche the Data the GUI Elments
						}

						GraphicalUserInterface::GraphicalUserInterface()
						{

						}

						void GraphicalUserInterface::show(int cmdShow) {

						}
					}
				}

				namespace proxy
				{
					void UserInterfaceObserver::notifyImpl(std::shared_ptr<abstraction::data::Data> eventData)
					{
						auto data = std::dynamic_pointer_cast<view::data::UserInterfaceIntputData>(eventData);
						if (!data)
						{
							throw abstraction::data::exception::Exception("Could not convert CommandData to a command");
						}
						else
						{
							m_ce.commandEntered(data->getData(), data->getSender());
						}
					}
				}

				namespace device_input_output
				{
					// RAS
				}
			}
		} // namespace view

		namespace controller
		{
			namespace data
			{
				class CommandRepository::CommandRepositoryImpl
				{
				public:
					CommandRepositoryImpl();
					void registerCommand(const string& name, abstraction::data::command::unique_command_ptr c);
					abstraction::data::command::unique_command_ptr deregisterCommand(const string& name);

					size_t count() const { return m_repository.size(); }
					abstraction::data::command::unique_command_ptr getCommandByName(const string& name) const;

					bool hasKey(const string& s) const;
					set<string> getAllCommandNames() const;

					void printHelp(const std::string& command, std::ostream& os);

					void clearAllCommands();

				private:
					using Repository = unordered_map<string, abstraction::data::command::unique_command_ptr>;
					Repository m_repository;
				};

				CommandRepository::CommandRepositoryImpl::CommandRepositoryImpl()
				{

				}

				bool CommandRepository::CommandRepositoryImpl::hasKey(const string& s) const
				{
					return m_repository.find(s) != m_repository.end();
				}

				set<string> CommandRepository::CommandRepositoryImpl::getAllCommandNames() const
				{
					set<string> tmp;

					for (auto i = m_repository.begin(); i != m_repository.end(); ++i)
						tmp.insert(i->first);

					return tmp;
				}

				void CommandRepository::CommandRepositoryImpl::printHelp(const std::string& command, std::ostream& os)
				{
					auto it = m_repository.find(command);
					if (it != m_repository.end())
						os << command << ": " << it->second->getHelpMessage();
					else
						os << command << ": no help entry found";
					return;
				}

				void CommandRepository::CommandRepositoryImpl::clearAllCommands()
				{
					m_repository.clear();
					return;
				}

				void CommandRepository::CommandRepositoryImpl::registerCommand(const string& name, abstraction::data::command::unique_command_ptr c)
				{
					if (hasKey(name))
					{
						std::ostringstream oss;
						oss << "Command " << name << " already registered";
						throw abstraction::data::exception::Exception{ oss.str() };
					}
					else
						m_repository.emplace(name, std::move(c));

					return;
				}

				abstraction::data::command::unique_command_ptr CommandRepository::CommandRepositoryImpl::deregisterCommand(const string& name)
				{
					if (hasKey(name))
					{
						auto i = m_repository.find(name);
						auto tmp = make_unique_command_ptr(i->second.release());
						m_repository.erase(i);
						return tmp;
					}
					else
						return abstraction::data::command::make_unique_command_ptr(nullptr);
				}

				abstraction::data::command::unique_command_ptr CommandRepository::CommandRepositoryImpl::getCommandByName(const string& name) const
				{
					if (hasKey(name))
					{
						const auto& command = m_repository.find(name)->second;
						return make_unique_command_ptr(command->clone());
					}
					else
						return abstraction::data::command::make_unique_command_ptr(nullptr);
				}

				CommandRepository::CommandRepository()
					: pimpl_{ new CommandRepositoryImpl }
				{
				}

				CommandRepository::~CommandRepository()
				{
				}

				CommandRepository& CommandRepository::getInstance()
				{
					static CommandRepository instance;
					return instance;
				}

				void CommandRepository::registerCommand(const string& name, abstraction::data::command::unique_command_ptr c)
				{
					pimpl_->registerCommand(name, std::move(c));

					return;
				}
				abstraction::data::command::unique_command_ptr CommandRepository::deregisterCommand(const string& name)
				{
					return pimpl_->deregisterCommand(name);
				}

				size_t CommandRepository::count() const
				{
					return pimpl_->count();
				}

				abstraction::data::command::unique_command_ptr CommandRepository::getCommandByName(const string& name) const
				{
					return pimpl_->getCommandByName(name);
				}

				bool CommandRepository::hasKey(const string& s) const
				{
					return pimpl_->hasKey(s);
				}

				set<string> CommandRepository::getAllCommandNames() const
				{
					return pimpl_->getAllCommandNames();
				}

				void CommandRepository::printHelp(const std::string& command, std::ostream& os) const
				{
					pimpl_->printHelp(command, os);
					return;
				}

				void CommandRepository::clearAllCommands()
				{
					pimpl_->clearAllCommands();
					return;
				}
			}

			namespace control
			{
				namespace coordinator
				{
					void ClientCoordinator::executeCommand(abstraction::data::command::unique_command_ptr c) {
						m_server_coordinator.executeCommand(std::move(c));
					}

					void ClientCoordinator::undo() {
						m_server_coordinator.undo();
					}
					void ClientCoordinator::refresh() {
						m_server_coordinator.refresh();
					}

					void ClientCoordinator::redo() {
						m_server_coordinator.redo();
					}
				}

				namespace state_dependent_control
				{
					class CommandDispatcher::CommandDispatcherImpl
					{
					public:
						explicit CommandDispatcherImpl(client_subsystem::view::boundary::user_interaction::UserInterface& ui);
						void executeCommand(const string& command, const string& sender);

					private:
						coordinator::ClientCoordinator clientCoordinator;
						client_subsystem::view::boundary::user_interaction::UserInterface& m_ui;
					};

					CommandDispatcher::CommandDispatcherImpl::CommandDispatcherImpl(client_subsystem::view::boundary::user_interaction::UserInterface& ui)
						: m_ui(ui)
					{
					}

					void CommandDispatcher::CommandDispatcherImpl::executeCommand(const string& command, const string& sender)
					{
						if (command == "exit")
							return;
						else if (command == "undo")
						{
							clientCoordinator.undo();
						}
						else if (command == "redo")
						{
							clientCoordinator.redo();
						}
						else if (sender == "timer")
							clientCoordinator.executeCommand(abstraction::data::command::make_unique_command_ptr(new data_abstraction::RotateCommand("hoursHand", stof(command))));
						else
						{
							auto c = data::CommandRepository::getInstance().getCommandByName(sender);
							if (!c)
							{
								ostringstream oss;
								oss << "Command " << command << " is not a known command";
								m_ui.sendOutput(oss.str().c_str());
							}
						}
					}

					CommandDispatcher::~CommandDispatcher()
					{
					}

					CommandDispatcher& CommandDispatcher::getInstance(client_subsystem::view::boundary::user_interaction::UserInterface& ui)
					{
						static CommandDispatcher instance(ui);
						return instance;
					}

					void CommandDispatcher::commandEntered(const std::string& command, const std::string& sender)
					{
						pimpl_->executeCommand(command, sender);
					}

					CommandDispatcher::CommandDispatcher(client_subsystem::view::boundary::user_interaction::UserInterface& ui)
					{
						pimpl_ = std::make_unique<CommandDispatcherImpl>(ui);
					}
				}

				namespace timer
				{
					// RAS
				}
			}

		} // namespace controller
	}

	void Facade::run()
	{
		using namespace client_subsystem::view::boundary;
		user_interaction::cli::CustomerInteraction ci(std::cin, std::cout);

		proxy::UserInterfaceObserver ui_observer(client_subsystem::controller::control::state_dependent_control::CommandDispatcher::getInstance(ci));

		ci.subscribe(user_interaction::gui::GraphicalUserInterface::InputEntered, std::make_unique<proxy::UserInterfaceObserver>(ui_observer));

		using namespace server_subsystem::boundary::proxy;
		ModelObserver model_observer(ci);
		ModelProxy::getInstance().subscribe(ModelProxy::resultAvailable, std::make_unique<ModelObserver>(model_observer));

		ci.run();

		//proxy::UserInterfaceObserver ui_observer(client_subsystem::controller::control::state_dependent_control::CommandDispatcher::getInstance(user_interaction::gui::GraphicalUserInterface::getInstance()));
		//user_interaction::gui::GraphicalUserInterface::getInstance().subscribe(user_interaction::gui::GraphicalUserInterface::InputEntered, std::make_unique<proxy::UserInterfaceObserver>(ui_observer));

		//using namespace server_subsystem::boundary::proxy;
		//ModelObserver model_observer(user_interaction::gui::GraphicalUserInterface::getInstance());
		//ModelProxy::getInstance().subscribe(ModelProxy::resultAvailable, std::make_unique<ModelObserver>(model_observer));
	}
}
