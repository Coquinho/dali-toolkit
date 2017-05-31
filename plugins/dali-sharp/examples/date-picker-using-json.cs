/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

using System;
using System.Runtime.InteropServices;
using Dali;

namespace MyCSharpExample
{
    // A spin control (for continously changing values when users can easily predict a set of values)

    class Example
    {
        private Dali.Application _application;
        private Spin _spinYear;  // spin control for year
        private Spin _spinMonth; // spin control for month
        private Spin _spinDay;   // spin control for day
        private Builder _builder; // DALi Builder

        public Example(Dali.Application application)
        {
            _application = application;
            _application.Initialized += Initialize;
        }

        public void Initialize(object source, NUIApplicationInitEventArgs e)
        {

            Stage stage = Stage.GetCurrent();
            stage.BackgroundColor = Color.White;

            // load date JSON template...

            _builder = new Builder ();

            // Optional constant to see logging information coming out
            // of DALi JSON parser (builder)
            Property.Map constants = new  Property.Map();
            constants.Insert( "CONFIG_SCRIPT_LOG_LEVEL",  new Property.Value( "Verbose") );
            _builder.AddConstants( constants );

            _builder.LoadFromFile( "./json/date-picker-template.json" );

            // create the date-picker from the template in the json file
            BaseHandle handle =  _builder.Create( "date-picker");

            Actor actorTree =  Actor.DownCast( handle );

            stage.Add( actorTree );

            Actor year  = actorTree.FindChildByName("Year");
            Actor month  =  actorTree.FindChildByName("Month" );
            Actor day  = actorTree.FindChildByName("Day");

            // need to get the actual C# Spin object associated with the actor,
            _spinYear = View.DownCast<Spin>( year );
            _spinMonth = View.DownCast<Spin>( month );
            _spinDay = View.DownCast<Spin>( day );

            _spinYear.Value = 2099;
            _spinMonth.Value = 5;
            _spinDay.Value = 23;

            _spinYear.SetFocusable(true);
            _spinMonth.SetFocusable(true);
            _spinDay.SetFocusable(true);

            FocusManager FocusManager = FocusManager.Instance;
            FocusManager.PreFocusChange += OnPreFocusChange;
            FocusManager.FocusedActorEnterKeyPressed += OnFocusedActorEnterKeyPressed;

        }

        private Actor OnPreFocusChange(object source, FocusManager.PreFocusChangeEventArgs e)
        {
            Actor nextFocusActor = e.Proposed;

            // When nothing has been focused initially, focus the text field in the first spin
            if (!e.Current && !e.Proposed)
            {
                nextFocusActor = _spinYear.SpinText;
            }
            else if(e.Direction == View.Focus.Direction.LEFT)
            {
                // Move the focus to the spin in the left of the current focused spin
                if(e.Current == _spinMonth.SpinText)
                {
                    nextFocusActor = _spinYear.SpinText;
                }
                else if(e.Current == _spinDay.SpinText)
                {
                    nextFocusActor = _spinMonth.SpinText;
                }
            }
            else if(e.Direction == View.Focus.Direction.RIGHT)
            {
                // Move the focus to the spin in the right of the current focused spin
                if(e.Current == _spinYear.SpinText)
                {
                    nextFocusActor = _spinMonth.SpinText;
                }
                else if(e.Current == _spinMonth.SpinText)
                {
                    nextFocusActor = _spinDay.SpinText;
                }
            }

            return nextFocusActor;
        }

        private void OnFocusedActorEnterKeyPressed(object source, FocusManager.FocusedActorEnterKeyEventArgs e)
        {
            // Make the text field in the current focused spin to take the key input
            KeyInputFocusManager manager = KeyInputFocusManager.Get();

            if (e.Actor == _spinYear.SpinText)
            {
                if (manager.GetCurrentFocusControl() != _spinYear.SpinText)
                {
                    manager.SetFocus(_spinYear.SpinText);
                }
            }
            else if (e.Actor == _spinMonth.SpinText)
            {
                if (manager.GetCurrentFocusControl() != _spinMonth.SpinText)
                {
                    manager.SetFocus(_spinMonth.SpinText);
                }
            }
            else if (e.Actor == _spinDay.SpinText)
            {
                if (manager.GetCurrentFocusControl() != _spinDay.SpinText)
                {
                    manager.SetFocus(_spinDay.SpinText);
                }
            }
        }

        public void MainLoop()
        {
            _application.MainLoop ();
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            Example example = new Example(Application.NewApplication());
            example.MainLoop ();
        }
    }
}
