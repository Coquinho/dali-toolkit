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
    class Example
    {
        private Dali.Application _application;
        private ScrollView _scrollView;
        private ScrollBar _scrollBar;
        private Animation _animation;
        private TextLabel _text;

        public Example(Dali.Application application)
        {
            _application = application;
            _application.Initialized += Initialize;
        }

        public void Initialize(object source, NUIApplicationInitEventArgs e)
        {
            CreateScrollView();
        }

        private void CreateScrollView()
        {
            Window window = Window.Instance;
            window.BackgroundColor = Color.White;

            // Create a scroll view
            _scrollView = new ScrollView();
            Size2D windowSize = window.Size;
            _scrollView.Size = new Position(windowSize.Width, windowSize.Height, 0.0f);
            _scrollView.ParentOrigin = NDalic.ParentOriginCenter;
            _scrollView.PivotPoint = NDalic.AnchorPointCenter;
            window.Add(_scrollView);

            // Add actors to a scroll view with 3 pages
            int pageRows = 1;
            int pageColumns = 3;
            for (int pageRow = 0; pageRow < pageRows; pageRow++)
            {
                for (int pageColumn = 0; pageColumn < pageColumns; pageColumn++)
                {
                    View pageActor = new View();
                    pageActor.SetResizePolicy(ResizePolicyType.FILL_TO_PARENT, DimensionType.AllDimensions);
                    pageActor.ParentOrigin = NDalic.ParentOriginCenter;
                    pageActor.PivotPoint = NDalic.AnchorPointCenter;
                    pageActor.Position = new Position(pageColumn * windowSize.Width, pageRow * windowSize.Height, 0.0f);

                    // Add images in a 3x4 grid layout for each page
                    int imageRows = 4;
                    int imageColumns = 3;
                    float margin = 10.0f;
                    Position imageSize = new Position((windowSize.Width / imageColumns) - margin, (windowSize.Height / imageRows) - margin, 0.0f);

                    for (int row = 0; row < imageRows; row++)
                    {
                        for (int column = 0; column < imageColumns; column++)
                        {
                            int imageId = (row * imageColumns + column) % 2 + 1;
                            ImageView imageView = new ImageView("images/image-" + imageId + ".jpg");
                            imageView.ParentOrigin = NDalic.ParentOriginCenter;
                            imageView.PivotPoint = NDalic.AnchorPointCenter;
                            imageView.Size = imageSize;
                            imageView.Position = new Position(margin * 0.5f + (imageSize.X + margin) * column - windowSize.Width * 0.5f + imageSize.X * 0.5f,
                                                 margin * 0.5f + (imageSize.Y + margin) * row - windowSize.Height * 0.5f + imageSize.Y * 0.5f, 0.0f);
                            pageActor.Add(imageView);
                        }
                    }

                    _scrollView.Add(pageActor);
                }
            }

            _scrollView.SetAxisAutoLock(true);

            // Set scroll view to have 3 pages in X axis and allow page snapping,
            // and also disable scrolling in Y axis.
            Property.Map rulerMap = new Property.Map();
            rulerMap.Add((int)Dali.Constants.ScrollModeType.XAxisScrollEnabled, new Property.Value(true));
            rulerMap.Add((int)Dali.Constants.ScrollModeType.XAxisSnapToInterval, new Property.Value(windowSize.Width));
            rulerMap.Add((int)Dali.Constants.ScrollModeType.XAxisScrollBoundary, new Property.Value(windowSize.Width * pageColumns ) );
            rulerMap.Add((int)Dali.Constants.ScrollModeType.YAxisScrollEnabled, new Property.Value( false ) );
            _scrollView.ScrollMode = rulerMap;

            // Create a horizontal scroll bar in the bottom of scroll view (which is optional)
            _scrollBar = new ScrollBar();
            _scrollBar.ParentOrigin = NDalic.ParentOriginBottomLeft;
            _scrollBar.PivotPoint = NDalic.AnchorPointTopLeft;
            _scrollBar.SetResizePolicy(ResizePolicyType.FIT_TO_CHILDREN, DimensionType.Width);
            _scrollBar.SetResizePolicy(ResizePolicyType.FILL_TO_PARENT, DimensionType.Height);
            _scrollBar.Orientation = new Rotation(new Radian(new Degree(270.0f)), Vector3.ZAXIS);
            _scrollBar.SetScrollDirection(ScrollBar.Direction.Horizontal);
            _scrollView.Add(_scrollBar);

            // Connect to the OnRelayout signal
            _scrollView.Relayout += OnScrollViewRelayout;
            _scrollView.Touched += OnTouch;
            _scrollView.WheelRolled += Onwheel;
            _scrollView.FocusGained += OnKey;
            _text = new TextLabel("View Touch Event Handler Test");
            _text.PivotPoint = NDalic.AnchorPointTopLeft;
            _text.HorizontalAlignment = "CENTER";
            _text.PointSize = 48.0f;

            _scrollView.Add(_text);
        }

        // Callback for _animation finished signal handling
        public void AnimationFinished(object sender, EventArgs e)
        {
            Console.WriteLine("Customized Animation Finished Event handler");
        }

        private void OnKey(object source, View.KeyInputFocusGainedEventArgs e)
        {
            Console.WriteLine("View Keyevent EVENT callback....");
        }

        private bool Onwheel(object source, View.WheelEventArgs e)
        {
            Console.WriteLine("View Wheel EVENT callback....");
            return true;
        }

        private bool OnTouch(object source, View.TouchEventArgs e)
        {
            Console.WriteLine("View TOUCH EVENT callback....");

            // Only animate the _text label when touch down happens
            if (e.Touch.GetState(0) == PointStateType.DOWN)
            {
                Console.WriteLine("Customized Window Touch event handler");
                // Create a new _animation
                if (_animation)
                {
                    _animation.Reset();
                }

                _animation = new Animation(1.0f); // 1 second of duration

                _animation.AnimateTo(new Property(_text, View.Property.ORIENTATION), new Property.Value(new Rotation(new Radian(new Degree(180.0f)), Vector3.XAXIS)), new AlphaFunction(AlphaFunction.BuiltinFunction.Linear), new TimePeriod(0.0f, 0.5f));
                _animation.AnimateTo(new Property(_text, View.Property.ORIENTATION), new Property.Value(new Rotation(new Radian(new Degree(0.0f)), Vector3.XAXIS)), new AlphaFunction(AlphaFunction.BuiltinFunction.Linear), new TimePeriod(0.5f, 0.5f));

                // Connect the signal callback for animaiton finished signal
                _animation.Finished += AnimationFinished;

                // Play the _animation
                _animation.Play();
            }
            return true;
        }

        private void OnScrollViewRelayout(object source, View.OnRelayoutEventArgs e)
        {
            Console.WriteLine("View OnRelayoutEventArgs EVENT callback....");

            // Set the correct scroll bar size after size negotiation of scroll view is done
            _scrollBar.Size = new Position(0.0f, _scrollView.GetRelayoutSize(DimensionType.Width), 0.0f);
        }

        public void MainLoop()
        {
            _application.MainLoop();
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            Example example = new Example(Application.NewApplication());
            example.MainLoop();
        }
    }
}
