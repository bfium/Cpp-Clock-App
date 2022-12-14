#pragma once

#include<Windows.h>
#include <sstream>
#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include <stack>
#include <memory>
#include <vector>

	namespace external
	{
		namespace physical
		{
			namespace input_output
			{
				// RAS
			}

			namespace input
			{
				// RAS
			}

			namespace output
			{
				// RAS
			}

			namespace user
			{
				// RAS
			}
		}
	}

	namespace abstraction
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

					void undo() { return undoImpl(); }
					void execute()
					{
						checkPreConditionImpl();
						executeImpl();
						checkPostConditionImpl();
					}
					Command* clone() const { return cloneImpl(); }
					virtual void deallocate() { delete this; }
					const char* getHelpMessage() const { return getHelpMessageImpl(); }

				protected:
					Command() = default;
					Command(const Command&) = default;

					virtual void undoImpl() noexcept = 0;
					virtual void executeImpl() noexcept = 0;
					virtual Command* cloneImpl() const noexcept = 0;

					virtual void checkPostConditionImpl() const {}
					virtual void checkPreConditionImpl() const {}
					virtual const char* getHelpMessageImpl() const noexcept = 0;

				private:
					Command(Command&&) = delete;
					Command& operator=(const Command&) = delete;
					Command& operator=(Command&&) = delete;
				};

				/*
					must go in the Algoritm namespace
				*/
				inline void deallocate(Command* cmd)
				{
					if (cmd)
						cmd->deallocate();
				}

				using unique_command_ptr = std::unique_ptr<Command, decltype(&deallocate)>;

				inline auto make_unique_command_ptr(Command* c)
				{
					return unique_command_ptr{ c, &deallocate };
				}

				// 2: Creational Pattern: Abstract Factory
				class CommandFactory
				{
				public:
					virtual ~CommandFactory() = default;
					virtual unique_command_ptr create() const
					{
						return createImpl();
					}

				protected:
					CommandFactory() = default;
					CommandFactory(const CommandFactory&) = default;

				protected:
					virtual unique_command_ptr createImpl() const = 0;

				private:
					CommandFactory(CommandFactory&&) = delete;
					CommandFactory& operator=(const CommandFactory&) = delete;
					CommandFactory& operator=(CommandFactory&&) = delete;
				};
			}

			namespace exception
			{
				class Exception
				{
				public:
					Exception(const std::string& message) : m_msg{ message } {}
					const std::string& what() const { return m_msg; }

				private:
					std::string m_msg;
				};
			}

		} // namespace _system

		namespace logic
		{
			namespace business
			{
				// Class Command;
				class Manager
				{
				public:
					virtual ~Manager() = default;
					virtual void execute(data::command::Command* data) noexcept = 0;
					// virtual const char *execute(data::InputData *data) noexcept = 0;
				};
			}
			namespace algorithm
			{
				// RAS
			}

			namespace service
			{
				class Service
				{
				public:
					virtual ~Service() = default;
					virtual std::string getName() const = 0;
					virtual abstraction::data::OutputData* transform(std::shared_ptr<abstraction::data::InputData> d) = 0;
					virtual const std::string getServiceDescription() const = 0;
					virtual const std::string getServiceLocalisation() const = 0;
				};
			}

		} // namespace logic

		namespace boundary
		{
			namespace user_interaction
			{
				class UserInteraction
				{
				public:
					virtual ~UserInteraction() = default;
					virtual void sendInput() = 0;
					virtual void sendOutput(const char*) = 0;
					virtual void sendOutput(std::shared_ptr<abstraction::data::Data>d) = 0;

				};
			}

			namespace proxy
			{
				class Observer
				{
				public:
					Observer(const std::string& n) : name{ n } {}
					const std::string& getName() const { return name; }
					virtual void notify(std::shared_ptr<abstraction::data::Data> d) { notifyImpl(d); };
					virtual ~Observer() = default;

				private:
					virtual void notifyImpl(std::shared_ptr<abstraction::data::Data> d) = 0;

				private:
					std::string name;
				};
			}

			namespace device_input_output
			{

			}
		}
	} // namespace abstraction

	namespace service_system
	{
		namespace html
		{
			namespace data
			{
				class HtmlOutputData : public abstraction::data::OutputData
				{
				public:
					HtmlOutputData(const std::string& d) : m_data{ d } {};
					~HtmlOutputData() = default;
					const std::string getData() const { return m_data; }

				private:
					std::string m_data;
				};

				class HtmlInputData : public abstraction::data::InputData
				{
				public:
					HtmlInputData(const std::string& d) : m_data{ d } {};
					~HtmlInputData() = default;
					std::string getData() const { return m_data; }

				private:
					std::string m_data;
				};

#define CHAR_LENGTH 3
				static const char tr[CHAR_LENGTH] = "tr";
				static const char td[CHAR_LENGTH] = "td";
				static const char th[CHAR_LENGTH] = "th";
				static const char table[CHAR_LENGTH + 7] = "table";
				static const char a = 'a';

				using TagName = std::string;
				using TagContent = std::string;

				class TagBase
				{
				public:
					virtual~TagBase() = default;
					virtual const std::string str() const noexcept = 0;

				protected:
					TagBase() = default;
					TagBase(const TagName& n, const TagContent& c = "") :name{ n }, content{ c }{}
					const TagName getName()const { return name; };
					const TagContent getContent()const { return content; };

				private:
					TagName name{};
					TagContent content{};
				};

				struct Attribute
				{
					std::string name;
					std::string value;
				};

				class Tag : public TagBase
				{
				public:
					Tag() = default;
					Tag(const TagName& name, const TagContent& content = "")
						:TagBase(name, content),
						children{},
						attributes{}
					{}
					Tag(const TagName& root, std::initializer_list<Tag>& list)
						:TagBase(root, ""),
						children{ list },
						attributes{}
					{}
					Tag(const TagName& root, const TagContent& content, std::initializer_list<Tag>& list)
						:TagBase(root, content),
						children{ list },
						attributes{}
					{}

					virtual~Tag() = default;

					virtual const std::string str()const noexcept override
					{
						std::ostringstream oss;

						oss << "<" << getName();
						for (const auto& att : attributes)
							oss << " " << att.name << " =\"" << att.value << "\"";

						oss << ">" << getContent();

						for (const auto& child : children)
							oss << child.str();

						oss << "</" << getName() << ">" << "\n";

						return oss.str();

					}
					void addAttribute(const std::string& name, const std::string& value) noexcept
					{
						Attribute attr;
						attr.name = name;
						attr.value = value;
						attributes.push_back(attr);
					}
					void addChild(const Tag& tag) noexcept
					{
						children.emplace_back(tag);
					}
					std::vector<Tag> getChildren()const { return children; }
				protected:
					std::vector<Tag> children;
					std::vector<Attribute> attributes;
				};

				class Tr : public Tag
				{
				public:
					Tr(const TagContent& c = "") : Tag(tr, c) {}
					Tr(std::initializer_list<Tag>list) :Tag(tr, list) {}

					~Tr() = default;
				};

				class Td : public Tag
				{
				public:
					Td(const TagContent& c = "") : Tag(td, c) {}
					Td(std::initializer_list<Tag>list) :Tag(td, "", list) {}
					~Td() = default;
				};

				class Th : public Tag
				{
				public:
					Th(const TagContent& c) : Tag(th, c) {}
					~Th() = default;
				};

				class Style : public Tag
				{
				public:
					Style(const TagContent& c) : Tag("style", c) {}
					~Style() = default;
				};

				class A : public Tag
				{
				public:
					A(const std::string& link, const TagContent& c) : Tag("a", c)
					{
						addAttribute("href", link);
					}
					~A() = default;
				};
				class P : public Tag
				{
				public:
					P(const TagContent& c) : Tag("p", c) {}
					~P() = default;
				};

				class HtmlTable : public Tag
				{
				public:
					HtmlTable(std::initializer_list<Tag>list) : Tag(table, list) {}
					~HtmlTable() = default;

					const std::string str()const noexcept override
					{
						std::ostringstream oss;

						oss << "<" << getName() << " BORDER COLS = 2";
						oss << ">" << getContent();

						for (const auto& child : children)
							oss << child.str();

						oss << "</" << getName() << ">" << "\n";

						return oss.str();
					}
				};

				class HtmlPage
				{
				public:

					HtmlPage(bool h)
						:asHeader{ h },
						type{ "<!DOCTYPE HTML PUBLIC  -//W3C//DTD HTML 3.2//EN>" },
						head{ "head","" },
						html{ "html","" },
						body{ "body","" }
					{}

					~HtmlPage() = default;
					const std::string str()//const noexcept
					{
						body.addAttribute("style", "background-color:#afafaf;");

						std::ostringstream oss;
						if (asHeader)html.addChild(head);

						for (const auto& t : m_tags) body.addChild(t);

						html.addChild(body);

						oss << type << "\n"
							<< html.str();

						return oss.str();
					}

					void setBody(const std::vector<Tag>& v) {
						m_tags = v;
					}
					void setBody(const Tag& v) {
						m_tags.push_back(v);
					}
				private:
					std::vector<Tag> m_tags;
					Tag head;
					Tag html;
					Tag body;
					std::string type;
					bool asHeader{ false };

				};
			}

			//namespace boundary
			//{
			//	namespace proxy
			//	{
			//	} // namespace proxy
			//}

			namespace logic
			{
				namespace business
				{
					class HtmlManager
					{
					public:
						class HtmlBuilder
						{
						public:
							HtmlBuilder(html::data::HtmlPage& pg) : page{ pg } {}

							~HtmlBuilder() {}
							HtmlBuilder& body(const data::Tag& tag)
							{
								page.setBody(tag);
								return *this;
							}

							HtmlBuilder& body(const std::vector<data::Tag>& tag)
							{
								page.setBody(tag);
								//page.m_tags = { tag };
								return *this;
							}

						private:
							data::HtmlPage& page;
						};

						HtmlManager() = default;
						~HtmlManager() = default;
					};

				} // namespace business

				namespace service
				{
					/* This Service is not well designed
						I need an Intent Communication Builder or
						a Fluent API Builder
					 */
					class HtmlService : public abstraction::logic::service::Service
					{
					public:
						static const std::string name;
					public:
						HtmlService() = default;
						~HtmlService() = default;
						std::string getName() const override { return name; }
						abstraction::data::OutputData* transform(std::shared_ptr<abstraction::data::InputData> d) override
						{

							business::HtmlManager manager;
							std::string da{ "the result" };

							//auto r = manager.create(da);

							return new data::HtmlOutputData(da);
						}
						const std::string getServiceDescription() const override { return "create a Html page"; }
						const std::string getServiceLocalisation() const override { return "located somewhere"; }
					};
				}
			}
		}

		namespace logger
		{
			namespace logger_data_abstraction
			{
				class LoggerData
				{
				};
			}
			class Logger : public abstraction::logic::service::Service
			{
				// to be add in the concrete Logger
				static const std::string name;

			public:
				virtual ~Logger() = default;
				virtual void log(const char*) = 0;

			public:
				virtual std::string getName() const noexcept override { return "Logger"; };
				const std::string getServiceDescription() const override { return "This Class is use to log"; }
				const std::string getServiceLocalisation() const override { return "Located somewhere"; }
			};
		}

		namespace publisher
		{
			namespace publisher_data_abstraction
			{
				class PublisherData : public abstraction::data::Data
				{
				public:
				};
			}

			class Publisher : public abstraction::logic::service::Service
			{
				static const std::string name;
				class PublisherImpl;

			public:
				Publisher();
				void subscribe(const std::string& eventName, std::unique_ptr<abstraction::boundary::proxy::Observer> observer);
				void unsubscribe(const std::string& eventName, const std::string& observerName);
				std::string getName() const noexcept override { return name; }
				const std::string getServiceDescription() const override { return "This Class is used for subscription"; }
				const std::string getServiceLocalisation() const override { return "Located somewhere"; }
				abstraction::data::OutputData* transform(std::shared_ptr<abstraction::data::InputData> f) override
				{
					return nullptr;
				};

				virtual ~Publisher();
				void notify(const std::string& eventName, std::shared_ptr<abstraction::data::Data>) const;
				void registerEvent(const std::string& eventName);

			private:
				std::unique_ptr<PublisherImpl> impl;
			};
		}

		namespace tokenizer
		{
			class Tokenizer : public abstraction::logic::service::Service
			{
				static const std::string name; // = "tokenizer";
				using Token = std::string;
				using Tokens = std::vector<std::string>;
				using const_iterator = Tokens::const_iterator;

			public:
				explicit Tokenizer(const std::string&);
				explicit Tokenizer(const std::string&, char);
				explicit Tokenizer(std::istream& is) { tokenize(is); }
				std::string getName() const noexcept override { return name; }
				const std::string getServiceDescription() const override { return "tokenize the string"; }
				const std::string getServiceLocalisation() const override { return "located somewhere"; }
				abstraction::data::OutputData* transform(std::shared_ptr<abstraction::data::InputData> d) override
				{
					return nullptr;
				};

				~Tokenizer();

			public:
				const_iterator begin() const { return tokens_.cbegin(); }
				const_iterator end() const { return tokens_.cend(); }
				const Token& operator[](size_t i) const { return tokens_[i]; }
				size_t size() const { return tokens_.size(); }

			private:
				void tokenize(std::istream&) noexcept;
				Tokens tokens_;

			private:
				Tokenizer() = delete;
				Tokenizer(const Tokenizer&) = delete;
				Tokenizer(Tokenizer&&) = delete;
				Tokenizer& operator=(const Tokenizer&) = delete;
				Tokenizer& operator=(Tokenizer&&) = delete;
			};
		}
	}

	namespace broker_system
	{
		namespace white_page
		{
			class BrokerForwarder
			{
			public:
				static BrokerForwarder& getInstance()
				{
					static BrokerForwarder instance;
					return instance;
				}
				~BrokerForwarder() = default;
				bool registerService(std::unique_ptr<abstraction::logic::service::Service> s) noexcept
				{
					return false;
				}
				abstraction::data::OutputData* forward(const std::string& serviceName, const abstraction::data::InputData& data) const noexcept;
			private:
				BrokerForwarder() : m_services{}
				{
					m_services.insert(std::make_pair("html", std::make_unique<service_system::html::logic::service::HtmlService>()));
				}
				std::unordered_map<std::string, std::unique_ptr<abstraction::logic::service::Service>> m_services;

			private:
				BrokerForwarder(const BrokerForwarder&) = delete;
				BrokerForwarder(BrokerForwarder&&) = delete;
				BrokerForwarder& operator=(const BrokerForwarder&) = delete;
				BrokerForwarder& operator=(BrokerForwarder&&) = delete;
			};
			class BrokerHandler
			{
			public:
				static BrokerHandler& getInstance()
				{
					static BrokerHandler instance;
					return instance;
				}
				~BrokerHandler() = default;
				bool registerService(std::shared_ptr<abstraction::logic::service::Service> s) noexcept;
				std::shared_ptr<abstraction::logic::service::Service> getService(const std::string& serviceName) const noexcept;
			private:
				BrokerHandler() : m_services{}
				{
					m_services.insert(std::make_pair("html", std::make_unique<service_system::html::logic::service::HtmlService>()));
				}
				std::unordered_map<std::string, std::shared_ptr<abstraction::logic::service::Service>> m_services;

			private:
				BrokerHandler(const BrokerHandler&) = delete;
				BrokerHandler(BrokerHandler&&) = delete;
				BrokerHandler& operator=(const BrokerHandler&) = delete;
				BrokerHandler& operator=(BrokerHandler&&) = delete;
			};

		}

		namespace yellow_page
		{
			enum class RegisteredServiceType
			{
				TYPE1,
				TYPE2,
				TYPE3,
			};

			class BrokerDiscoverer
			{
			public:
				static BrokerDiscoverer& getInstance()
				{
					static BrokerDiscoverer instance;
					return instance;
				}
				~BrokerDiscoverer() = default;
				bool registerService(RegisteredServiceType serviceType, std::unique_ptr<abstraction::logic::service::Service> s) noexcept;
				std::vector<std::string> getService(RegisteredServiceType serviceType) const;
				std::shared_ptr<abstraction::data::OutputData> forward(const std::string& serviceName, const abstraction::data::InputData& data) const noexcept;

			private:
				BrokerDiscoverer() : m_services{}
				{
				}
				std::vector<std::pair<RegisteredServiceType, std::unique_ptr<abstraction::logic::service::Service>>> m_services;

			private:
				BrokerDiscoverer(const BrokerDiscoverer&) = delete;
				BrokerDiscoverer(BrokerDiscoverer&&) = delete;
				BrokerDiscoverer& operator=(const BrokerDiscoverer&) = delete;
				BrokerDiscoverer& operator=(BrokerDiscoverer&&) = delete;
			};
		}
	}

	namespace app
	{
		namespace data_abstraction
		{
			class RotateCommand : public abstraction::data::command::Command
			{
			public:
				explicit RotateCommand(size_t id,float angle)
					: Command(), m_id{id},
					m_fAngle{ angle } {};

				RotateCommand(const RotateCommand& dC)
					:Command(dC),
					m_fAngle{ dC.m_fAngle } {}
				~RotateCommand() = default;

			protected:
				virtual void undoImpl()noexcept override;
				virtual void executeImpl()noexcept override;
				virtual RotateCommand* cloneImpl()const noexcept override { return new RotateCommand{ *this }; }

				//virtual	void checkPostConditionImpl()const override {};
				//virtual	void checkPreConditionImpl()const override {};
				virtual const char* getHelpMessageImpl()const noexcept override { return "Rotate the Hand"; };

			private:
				mutable float m_fAngle;
				size_t m_id;
			};
		}

		namespace logic
		{
			namespace service
			{

			}
		}

		// Model
		namespace server_subsystem
		{
			namespace data_abstraction
			{
				enum class ErrorType
				{
					EMPTY,
					TOO_FEW_ARGUMENT
				};
				class ModelOutputData : public abstraction::data::OutputData
				{
				public:
					ModelOutputData(const std::string& html): m_page(html) {}

					~ModelOutputData() = default;

					const std::string& getPage()const { return m_page; }

				private:
					std::string m_page;
				};

				class ModelProxyImpl
				{
					using Rectangle_ = std::string;
					using Model = std::unordered_map<int, std::vector<std::shared_ptr<Rectangle_>>>;
				public:
					using const_dCase_iterator = Model::const_iterator;
					using const_dCase_reference = Model::const_reference;

				public:
					~ModelProxyImpl() = default;
					ModelProxyImpl();

				public:
					const_dCase_iterator cbegin()const { return m_data.cbegin(); }
					const_dCase_iterator cend()const { return m_data.cend(); }
					void refresh()noexcept;

				private:
					void initialize()noexcept;
				private:
					Model m_data;
				private:
					ModelProxyImpl(const ModelProxyImpl&) = delete;
					ModelProxyImpl(ModelProxyImpl&&) = delete;
					ModelProxyImpl& operator=(const ModelProxyImpl&) = delete;
					ModelProxyImpl& operator=(ModelProxyImpl&&) = delete;
				};
			}

			namespace boundary
			{
				namespace user_interaction
				{
					// RAS
				}

				namespace proxy
				{
					class ModelObserver : public abstraction::boundary::proxy::Observer
					{
					public:
						explicit ModelObserver(abstraction::boundary::user_interaction::UserInteraction& ui)
							: Observer("ModelObserver"),
							m_ui{ ui }
						{}

					private:
						void notifyImpl(std::shared_ptr<abstraction::data::Data>) override;

					private:
						abstraction::boundary::user_interaction::UserInteraction& m_ui;
					};

					enum class HandID
					{
						HOURS,
						SECOND,
						MINUTS
					};

					class ModelProxy :/*private data_abstraction::AdamProxyImpl,*/ protected service_system::publisher::Publisher
					{
					public:
						static const std::string resultAvailable;
						static const std::string adamError;

					public:
						using Publisher::subscribe;
						using Publisher::unsubscribe;

					public:
						static ModelProxy& getInstance();

						void rotate(size_t id, float angle, bool notify)noexcept;
					private:
						ModelProxy();

					private:
						data_abstraction::ModelProxyImpl m_data;
						std::shared_ptr<abstraction::logic::service::Service> m_service;
					};
				}

				namespace device_input_output
				{
					// RAS
				}
			}

			namespace control
			{
				namespace coordinator
				{
					// 5: Behavioral Pattern: Strategy
					class ServerCoordinator
					{
						class ServerCoordinatorImpl;
						class UndoRedoStackStrategy;
						class UndoRedoListStrategyVector;
						class UndoRedoListStrategy;

					public:
						enum class UndoRedoStrategy
						{
							ListStrategy,
							StackStrategy,
							ListStrategyVector
						};

					public:
						ServerCoordinator(UndoRedoStrategy st = UndoRedoStrategy::StackStrategy);
						~ServerCoordinator();

						void executeCommand(abstraction::data::command::unique_command_ptr c);
						size_t getUndoSize() const;
						size_t getRedoSize() const;

						void undo();
						void redo();
						void refresh();
					private:
						std::unique_ptr<ServerCoordinatorImpl> pimpl_;

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
					class HtmlServiceWrapper : protected service_system::html::logic::service::HtmlService
					{
					public:
						HtmlServiceWrapper() = default;
						~HtmlServiceWrapper() = default;

					private:

					};
				}
			}
		}

		namespace client_subsystem
		{
			namespace view
			{
				namespace boundary
				{
					namespace user_interaction
					{
						class UserInterface : public abstraction::boundary::user_interaction::UserInteraction, protected service_system::publisher::Publisher
						{
						public:
							static const char* InputEntered;

						public:
							UserInterface() { Publisher::registerEvent(InputEntered); }
							virtual ~UserInterface() = default;
							// virtual void sendInput() override;
							// virtual void sendOutput(const char *) override;

						public:
							using Publisher::notify;
							using Publisher::subscribe;
							using Publisher::unsubscribe;
						};
					}
				}
			}

			namespace controller
			{
				namespace data
				{
					class CommandRepository
					{
						class CommandRepositoryImpl;

					public:
						static CommandRepository& getInstance();

						void registerCommand(const std::string& name, abstraction::data::command::unique_command_ptr c);
						abstraction::data::command::unique_command_ptr deregisterCommand(const std::string& name);
						size_t count() const;
						abstraction::data::command::unique_command_ptr getCommandByName(const std::string& name) const;
						bool hasKey(const std::string& s) const;
						std::set<std::string> getAllCommandNames() const;
						void printHelp(const std::string& command, std::ostream&) const;
						void clearAllCommands();

					private:
						CommandRepository();
						~CommandRepository();
						std::unique_ptr<CommandRepositoryImpl> pimpl_;

					private:
						CommandRepository(CommandRepository&) = delete;
						CommandRepository(CommandRepository&&) = delete;
						CommandRepository& operator=(CommandRepository&) = delete;
						CommandRepository& operator=(CommandRepository&&) = delete;
					};

				} // namespace data

				namespace control
				{
					namespace coordinator
					{
						class ClientCoordinator
						{
						public:
							ClientCoordinator(/* args */) :m_server_coordinator{} {}
							~ClientCoordinator() {}
							void executeCommand(abstraction::data::command::unique_command_ptr c);
							void undo();
							void redo();
							void refresh();

						private:
							server_subsystem::control::coordinator::ServerCoordinator m_server_coordinator;
						};
					} // namespace coordinator

					namespace state_dependent_control
					{
						class CommandDispatcher
						{
							class CommandDispatcherImpl;

						public:
							~CommandDispatcher();
							static CommandDispatcher& getInstance(client_subsystem::view::boundary::user_interaction::UserInterface& ui);
							void commandEntered(const std::string& command, const std::string& sender);
						private:
							explicit CommandDispatcher(client_subsystem::view::boundary::user_interaction::UserInterface& ui);
							std::unique_ptr<CommandDispatcherImpl> pimpl_;

						private:
							CommandDispatcher(const CommandDispatcher&) = delete;
							CommandDispatcher(CommandDispatcher&&) = delete;
							CommandDispatcher& operator=(const CommandDispatcher&) = delete;
							CommandDispatcher& operator=(CommandDispatcher&&) = delete;
						};

					} // namespace state_dependent_control

					namespace timer
					{

					}
				} // namespace control
				namespace logic
				{
					namespace business_logic
					{
						// RAS
					}

					namespace algorithm
					{
						// RAS
					}

					namespace service
					{
						// RAS
					}
				}
			} // namespace controller

			namespace view
			{
				namespace data
				{
					class UserInterfaceIntputData : public abstraction::data::InputData
					{
					public:
						UserInterfaceIntputData(const std::string& userInput, const std::string& Sender = "")
							: uii{ userInput },
							sender_{ Sender } {}
						const std::string& getData() const { return uii; }
						const std::string& getSender() const { return sender_; }

					private:
						std::string uii;
						std::string sender_;
					};
					class UserInterfaceOutputData : public abstraction::data::OutputData
					{
					public:
						UserInterfaceOutputData(const std::string& userInput) : uii{ userInput } {}
						const std::string& getEventData() const { return uii; }

					private:
						std::string uii;
					};
					class UserInterfaceData : public abstraction::data::Data
					{
					public:
						UserInterfaceData(const std::string& userInput, const std::string& Sender = "")
							: uii{ userInput },
							sender_{ Sender } {}
						const std::string& getData() const { return uii; }
						const std::string& getSender() const { return sender_; }

					private:
						std::string uii;
						std::string sender_;
					};

				} // namespace data

				namespace boundary
				{
					namespace user_interaction
					{
						namespace cli
						{
							class CustomerInteraction : public UserInterface
							{
							public:
								CustomerInteraction(std::istream& is, std::ostream& os) : m_is{ is }, m_os{ os } {}
								~CustomerInteraction() = default;
								void run();

							private:
								void sendInput() override;
								void sendOutput(const char* msg) override;
								void sendOutput(std::shared_ptr<abstraction::data::Data>d)override;

							private:
								std::istream& m_is;
								std::ostream& m_os;

							private:
								CustomerInteraction(const CustomerInteraction&) = delete;
								CustomerInteraction(CustomerInteraction&&) = delete;
								CustomerInteraction& operator=(const CustomerInteraction&) = delete;
								CustomerInteraction& operator=(CustomerInteraction&&) = delete;
							};
						} // namespace cli

						namespace gui {
							enum WindowID
							{
								BUTTON_UNDO,
								BUTTON_REDO,
								LABEL_EMPTY,

								//------------Popup-----------------//
								HTML_POPUP_MENU,
								BUTTON_OPEN_FILTER,
							};
#define MAX_BYTE 256
							typedef std::map<size_t, HWND> gid_map;

							class Win
							{
							public:
								HRESULT init();

							protected:
								Win() {};
							private:
								static LRESULT CALLBACK	WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

							private:
								void layout();
								mutable HWND m_hwnd;
							};

							class GraphicalUserInterface : public UserInterface, protected Win
							{
							public:
								static GraphicalUserInterface& getInstance()
								{
									static GraphicalUserInterface instance;
									return instance;
								}
								void show(int cmdShow);
							private:
								GraphicalUserInterface();
								void sendOutput(std::shared_ptr<abstraction::data::Data>d)override;

							private:
								void sendInput() override;
								void sendOutput(const char* msg) override;
							private:
								//user_interaction::gui::Win m_main_window;
							};
						}
					}	  // namespace user_interaction

					namespace proxy
					{
						class UserInterfaceObserver : public abstraction::boundary::proxy::Observer
						{
						public:
							explicit UserInterfaceObserver(controller::control::state_dependent_control::CommandDispatcher& ce)
								: Observer{ "UserInterfaceObserver" },
								m_ce{ ce }
							{};

						private:
							void notifyImpl(std::shared_ptr<abstraction::data::Data>) override;

							controller::control::state_dependent_control::CommandDispatcher& m_ce;
						};
					} // namespace proxy
				}

			} // namespace view
		}

		class Facade
		{
		public:
			Facade() {};
			static const char* getFacadeDescription() { return "the Facade of my system"; }
			void run();

		protected:
			virtual void start() {};
		};
	}

