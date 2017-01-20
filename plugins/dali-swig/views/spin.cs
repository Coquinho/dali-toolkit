﻿/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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

// A spin control (for continously changing values when users can easily predict a set of values)

namespace Dali
{
public class Spin : CustomView
  {
    private VisualBase _arrowVisual;
    private TextField _textField;
    private int _arrowVisualPropertyIndex;
    private string _arrowImage;
    private int _currentValue;
    private int _minValue;
    private int _maxValue;
    private int _singleStep;
    private bool _wrappingEnabled;
    private string _fontFamily;
    private string _fontStyle;
    private int _pointSize;
    private Color _textColor;
    private Color _textBackgroundColor;
    private int _maxTextLength;

    // Called by DALi Builder if it finds a Spin control in a JSON file
    static CustomView CreateInstance()
    {
      return new Spin();
    }

    // static constructor registers the control type (only runs once)
    static Spin()
    {
      // ViewRegistry registers control type with DALi type registery
      // also uses introspection to find any properties that need to be registered with type registry
      ViewRegistry.Instance.Register("Spin", CreateInstance, typeof(Spin) );
    }
    public Spin() : base(ViewWrapperImpl.CustomViewBehaviour.REQUIRES_KEYBOARD_NAVIGATION_SUPPORT | ViewWrapperImpl.CustomViewBehaviour.DISABLE_STYLE_CHANGE_SIGNALS)
    {

    }

    public override void OnInitialize()
    {
      // Initialize the propertiesControl
      _arrowImage = "./images/arrow.png";
      _textBackgroundColor = new Color(0.6f, 0.6f, 0.6f, 1.0f);
      _currentValue = 0;
      _minValue = 0;
      _maxValue = 0;
      _singleStep = 1;
      _maxTextLength = 0;

      // Create image visual for the arrow keys
      _arrowVisualPropertyIndex = RegisterProperty("ArrowImage", new Dali.Property.Value(_arrowImage), Dali.Property.AccessMode.READ_WRITE);
      _arrowVisual =  VisualFactory.Get().CreateVisual( _arrowImage, new Uint16Pair(150, 150) );
      RegisterVisual( _arrowVisualPropertyIndex, _arrowVisual );

      // Create a text field
      _textField = new TextField();
      _textField.ParentOrigin = NDalic.ParentOriginCenter;
      _textField.AnchorPoint = NDalic.AnchorPointCenter;
      _textField.WidthResizePolicy = "SIZE_RELATIVE_TO_PARENT";
      _textField.HeightResizePolicy = "SIZE_RELATIVE_TO_PARENT";
      _textField.SizeModeFactor = new Vector3( 1.0f, 0.45f, 1.0f );
      _textField.PlaceholderText = "----";
      _textField.BackgroundColor = _textBackgroundColor;
      _textField.HorizontalAlignment = "Center";
      _textField.VerticalAlignment = "Center";
      _textField.SetKeyboardFocusable(true);
      _textField.Name = "_textField";

      this.Add(_textField);

      _textField.KeyInputFocusGained += TextFieldKeyInputFocusGained;
      _textField.KeyInputFocusLost += TextFieldKeyInputFocusLost;
    }

    public override Vector3 GetNaturalSize()
    {
      return new Vector3(150.0f, 150.0f, 0.0f);
    }

    public void TextFieldKeyInputFocusGained(object source, KeyInputFocusGainedEventArgs e)
    {
      // Make sure when the current spin that takes input focus also takes the keyboard focus
      // For example, when you tap the spin directly
      FocusManager.Instance.SetCurrentFocusActor(_textField);
    }

    public void TextFieldKeyInputFocusLost(object source, KeyInputFocusLostEventArgs e)
    {
      int previousValue = _currentValue;

      // If the input value is invalid, change it back to the previous valid value
      if(int.TryParse(_textField.Text, out _currentValue))
      {
        if (_currentValue < _minValue || _currentValue > _maxValue)
        {
          _currentValue = previousValue;
        }
      }
      else
      {
        _currentValue = previousValue;
      }

      // Otherwise take the new value
      this.Value = _currentValue;
    }

    public override Actor GetNextKeyboardFocusableActor(Actor currentFocusedActor, View.KeyboardFocus.Direction direction, bool loopEnabled)
    {
      // Respond to Up/Down keys to change the value while keeping the current spin focused
      Actor nextFocusedActor = currentFocusedActor;
      if (direction == View.KeyboardFocus.Direction.UP)
      {
        this.Value += this.Step;
        nextFocusedActor = _textField;
      }
      else if (direction == View.KeyboardFocus.Direction.DOWN)
      {
        this.Value -= this.Step;
        nextFocusedActor = _textField;
      }
      else
      {
        // Return a native empty handle as nothing can be focused in the left or right
        nextFocusedActor = new Actor();
        nextFocusedActor.Reset();
      }

      return nextFocusedActor;
    }


    [ScriptableProperty()]
    public int Value
    {
      get
      {
        return _currentValue;
      }
      set
      {

        Console.WriteLine ("Value set to "  + value );
        _currentValue = value;

        // Make sure no invalid value is accepted
        if (_currentValue < _minValue)
        {
          _currentValue = _minValue;
        }

        if (_currentValue > _maxValue)
        {
          _currentValue = _maxValue;
        }

        _textField.Text = _currentValue.ToString();
      }
    }
    // MinValue property of type int:
    [ScriptableProperty()]
    public int MinValue
    {
      get
      {
        return _minValue;
      }
      set
      {
        _minValue = value;
      }
    }

    // MaxValue property of type int:
    [ScriptableProperty()]
    public int MaxValue
    {
      get
      {
        return _maxValue;
      }
      set
      {
        _maxValue = value;
      }
    }

    // Step property of type int:
    [ScriptableProperty()]
    public int Step
    {
      get
      {
        return _singleStep;
      }
      set
      {
        _singleStep = value;
      }
    }

    // WrappingEnabled property of type bool:
    [ScriptableProperty()]
    public bool WrappingEnabled
    {
      get
      {
        return _wrappingEnabled;
      }
      set
      {
        _wrappingEnabled = value;
      }
    }

    // TextPointSize property of type int:
    [ScriptableProperty()]
    public int TextPointSize
    {
      get
      {
        return _pointSize;
      }
      set
      {
        _pointSize = value;
        _textField.PointSize = _pointSize;
      }
    }

    // TextColor property of type Color:
    [ScriptableProperty()]
    public Color TextColor
    {
      get
      {
        return _textColor;
      }
      set
      {
          Console.WriteLine ("TextColor set to "  + value.R + "," + value.G + ","+ value.B);

        _textColor = value;
        _textField.TextColor = _textColor;
      }
    }

    // MaxTextLength property of type int:
    [ScriptableProperty()]
    public int MaxTextLength
    {
      get
      {
        return _maxTextLength;
      }
      set
      {
        _maxTextLength = value;
        _textField.MaxLength = _maxTextLength;
      }
    }

    public TextField SpinText
    {
      get
      {
        return _textField;
      }
      set
      {
        _textField = value;
      }
    }

    // Indicator property of type string:
    public string IndicatorImage
    {
      get
      {
        return _arrowImage;
      }
      set
      {
        _arrowImage = value;
        _arrowVisual =  VisualFactory.Get().CreateVisual( _arrowImage, new Uint16Pair(150, 150) );
        RegisterVisual( _arrowVisualPropertyIndex, _arrowVisual );
      }
  }
}
}