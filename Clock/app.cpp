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
#include<iostream>
#include<algorithm>

namespace abstract
{
    namespace data
    {
        namespace command
        {
            std::unique_ptr<Command> CommandRepository::get(const std::string& d) const
            {
                //auto ptr = std::find(m_commands.cbegin(), m_commands.cend(), [&](auto& d) {return d->getName() == s; });
                //if (ptr != m_commands.cend())
                //{
                //   // return std::make_unique<Command*>(ptr->second->clone());// Command need to implement the clone function
                //}
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
        const std::string Publisher::name = "publisher";

        void Publisher::notify(const std::string& e, std::shared_ptr<abstract::data::InputData> d)const {
            //to do
        }
        void Publisher::subscribe(const std::string& e, std::unique_ptr<abstract::boundary::proxy::Observer> o) {
            // to do
        }
        std::unique_ptr<abstract::boundary::proxy::Observer> Publisher::unsubscribe(const std::string& event, const std::string& oName) {

            return nullptr;
        }
        void Publisher::registerEvent(const std::string&) {
            // to do
        }

    } // namespace Publisher

    namespace tokenizer
    {
        const std::string Tokenizer::name = "tokenizer";

        Tokenizer::Tokenizer(std::string s) {

        }

        void Tokenizer::tokenize(std::istream& is) noexcept
        {
        }

        const std::string& Tokenizer::getName() const { return name; }
        std::string Tokenizer::getLocation() const { return "tokenizer service"; }
        std::string Tokenizer::getDescription() const { return "used for tokenization"; }

    } // namespace tokenizer

} // namespace service_system

namespace app
{
    namespace data
    {
        namespace command
        {
            void RotateCommand::executeImpl() {
                std::cout << "rotate with : " << std::endl;
            }
        } // namespace command
    }// namespace data;

    namespace client
    {
        namespace data
        {
            std::unique_ptr<abstract::data::command::Command> RotateCommandFactory::create(std::unique_ptr< abstract::data::InputData> d) const
            {
                // auto dat = dynamic_cast<RotateInputData*>(std::move(d));
                // return std::make_unique<app::data::command::RotateCommand>(new app::data::command::RotateCommand(dat->m_angle));
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
                    void Cli::notify() {

                    }
                    void Cli::sendInput(const char*) {

                    }
                    void Cli::sendInput(std::unique_ptr<abstract::data::InputData>) {

                    }
                    void Cli::run() {

                    }

                } // namespace cli

                namespace gui
                {
                    /*
                    LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
                    {
                        switch (uMsg)
                        {
                        case WM_CREATE:
                            if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))		// initialize the Factory
                                return -1;	// The Creation of the Window failed.
                            return 0;

                        case WM_DESTROY:
                            PostQuitMessage(0);
                            return 0;


                        case WM_SIZE:
                            Resize();
                            return 0;

                        case WM_PAINT:
                            OnPaint();
                            return 0;

                        default:
                            return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
                        }
                        return TRUE;
                    }

                    void MainWindow::DrawClockHand(float fHandLength, float fAngle, float fStrockeWidth)
                    {
                        pRenderTarget->SetTransform(
                            D2D1::Matrix3x2F::Rotation(fAngle, ellipse.point)
                        );

                        // endPoint defines one end of the hand.
                        D2D_POINT_2F endPoint = D2D1::Point2F(
                            ellipse.point.x,
                            ellipse.point.y - (ellipse.radiusY * fHandLength)
                        );

                        // Draw a line from the center of the ellipse to endPoint.
                        pRenderTarget->DrawLine(
                            ellipse.point,
                            endPoint,
                            pBrush,
                            fStrockeWidth
                        );
                    }

                    void MainWindow::RenderClock()
                    {
                        // Get the Size of the Render Target
                        D2D1_SIZE_F size = pRenderTarget->GetSize(); // returns the size of the render target in DIPs (not pixels), which is the appropriate unit for calculating layout.

                        // get the Center of the Render Target
                        const float x = size.width / 2;
                        const float y = size.height / 2;

                        // the radius of the ellipse according to the width and the heigh
                        const float radius = min(x, y);

                        // create the Ellipse
                        ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);


                        pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

                        pRenderTarget->FillEllipse(ellipse, pBrush);

                        // Draw hands
                        SYSTEMTIME time;
                        GetLocalTime(&time);

                        // 60 minutes = 30 degrees, 1 minute = 0.5 degree
                        const float fHourAngle = (360.0f / 12) * (time.wHour) + (time.wMinute * 0.5f);
                        const float fMinuteAngle = (360.0f / 60) * (time.wMinute);

                        DrawClockHand(0.6f, fHourAngle, 6);
                        DrawClockHand(0.85f, fMinuteAngle, 4);

                        // Restore the identity transformation.
                        pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
                    }

                    HRESULT MainWindow::CreateGraphicsResource()
                    {
                        HRESULT hr = S_OK;
                        if (pRenderTarget == NULL)
                        {
                            RECT rc;
                            GetClientRect(m_hwnd, &rc);

                            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

                            hr = pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hwnd, size),
                                &pRenderTarget);

                            if (SUCCEEDED(hr))
                            {
                                const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0.0f, 0.5f);
                                const D2D1_COLOR_F handColor = D2D1::ColorF(1.0f, 0.0f, 0.0f, 0.5f);
                                /*
                                    // Initialize a magenta color.
                                    D2D1_COLOR_F clr;
                                    clr.r = 1;
                                    clr.g = 0;
                                    clr.b = 1;
                                    clr.a = 1;  // Opaque.
                                *//*
                                hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
                                hr = pRenderTarget->CreateSolidColorBrush(handColor, &pBrushHand);

                                if (FAILED(hr))
                                {
                                    DiscardGraphicsResource();
                                }
                            }

                        }
                        return hr;
                    }

                    void MainWindow::DiscardGraphicsResource()
                    {
                        SafeRelease(&pRenderTarget);
                        SafeRelease(&pBrush);
                    }

                    void MainWindow::OnPaint()
                    {
                        HRESULT hr = CreateGraphicsResource();

                        if (SUCCEEDED(hr))
                        {
                            PAINTSTRUCT ps;

                            BeginPaint(m_hwnd, &ps);

                            pRenderTarget->BeginDraw();
                            pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));		// Draw Command 1
                            RenderClock();
                            hr = pRenderTarget->EndDraw();		// check if the Draw was successful

                            // Direct2D signals a lost device by returning the error code D2DERR_RECREATE_TARGET from the EndDraw method. 
                            // If you receive this error code, you must re-create the render target and all device-dependent resources.
                            if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
                            {
                                DiscardGraphicsResource();
                            }

                            EndPaint(m_hwnd, &ps);
                        }
                    }

                    void MainWindow::Resize()
                    {

                        if (pRenderTarget != NULL)
                        {
                            RECT rc;
                            GetClientRect(m_hwnd, &rc);

                            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

                            pRenderTarget->Resize(size);
                            RenderClock();



                            InvalidateRect(m_hwnd, NULL, FALSE);
                        }
                    }
                    */
                
                } // namespace gui
            }     // namespace user_interaction
        }         // namespace view

        namespace controller
        {
            namespace state_dependent_control
            {
                void ClockDispatcher::dispatch(const char* cmd, const char* sender) noexcept {

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
                void ClockManager::manage(std::shared_ptr<abstract::data::command::Command> c) noexcept {

                }
                void ClockManager::undo() {

                }
                void ClockManager::redo() {

                }
            } // namespace business

        } // namespace service

        namespace boundary
        {
            namespace proxy
            {
                void ModelObserver::notifyImpl(std::shared_ptr<abstract::data::InputData> d)
                {
#ifdef _DEBUG
                    std::cout << "ModelObserver::notifyImpl()" << std::endl;
#endif // _DEBUG

                }
            } // namespace proxy

        } // namespace boundary

    } // namespace server

    void Facade::run() {
#ifdef _DEBUG
        std::cout << "Facade::run()" << std::endl;
#endif // _DEBUG

    }
} // namespace app
