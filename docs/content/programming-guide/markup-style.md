<!--
/**-->

# Mark-up Style {#markup-style}

Mark-up tags can be used within the text to set styles.

By default the text controls don't process the mark-up string. To enable the mark-up string processing the property *ENABLE_MARKUP* must be set to *true*.

~~~{.cpp}
// C++

TextField field = TextField::New();
field.SetProperty( TextField::Property::ENABLE_MARKUP, true );

Stage::GetCurrent().Add( field );
~~~

Note the mark-up processor doesn't check the correctness of the mark-up string. This may
cause the text to be badly rendered.

The table below describes the priorities when styles are applied while rendering text.
|  |  |  |  |
|--|--|--|--|
| Priority 1 | Style set by markup string. | Will override the style set through the control properties. | i.e The \<color\> tag will override the *TEXT_COLOR* property. |
| Priority 2 | Style set through the control properties. | Will override the default platform style. |  |
| Priority 3 | Default platform style. |  |  |

Font size has slightly different priorities - the size provided by the platform is a logical
size, and can be mapped to physical point sizes using style sheets. There is a default set of
sizes defined for DALi, and these can be overridden by application specific stylesheets. Thus
the priorities are:

|  |  |  |
|--|--|--|
| Priority 1 | Size set by markup string. | Will override the style set through the stylesheets. |
| Priority 2 | Physical Size set by application style sheet | |
| Priority 2 | Logical Size set by application style sheet | Mapping from platform logical to physical |
| Priority 3 | Logical Size set by DALi style sheet | Mapping from platform logical to physical |

See [Font Selection](@ref font-selection) for more details.

Current supported tags are:

## \<color\>

Sets the color of the characters inside the tag. The *color* tag has a *value* attribute used to set the color. Possible values are: 'red', 'green', 'blue', 'yellow', 'magenta',
 'cyan', 'white', 'black' and 'transparent'. Web color and 32 bits hexadecimal 0xAARRGGBB formats are also supported.

Examples below are equivalent, render the text in red. Second example codes the color in 0xAARRGGBB, third and fourth in web color with 3 and 6 characters.

~~~{.cpp}
// C++
field.SetProperty( TextLabel::Property::TEXT, "<color value='red'>Red Text</color>" ); // Color coded with a text constant.
~~~

~~~{.cpp}
// C++
field.SetProperty( TextLabel::Property::TEXT, "<color value='0xFFFF0000'>Red Text</color>" ); // Color packed inside an ARGB hexadecimal value.
~~~

~~~{.cpp}
// C++
field.SetProperty( TextLabel::Property::TEXT, "<color value='#F00'>Red Text</color>" ); // Color packed with the web color format (3 characters).
~~~

~~~{.cpp}
// C++
field.SetProperty( TextLabel::Property::TEXT, "<color value='#FF0000'>Red Text</color>" ); // Color packed with the web color format (6 characters).
~~~

## \<font\>

Sets the font values of the characters inside the tag.

Supported attributes are:
- *family* The name of the font.
- *size* The size of the font in points.
- *weight* The weight of the font.
- *width* The width of the font
- *slant* The slant of the font.

See the [Font Selection](@ref font-selection) to have a view of the possible values for the *weight*, *width* and *slant* attributes.

~~~{.cpp}
// C++
field.SetProperty( TextLabel::Property::TEXT, "<font family='SamsungSans' weight='bold'>Hello world</font>" );
~~~

## XHTML ENTITIES

Single characters can be embedded into text using character entity references. These references have a numeric value as well as a named value.
You can use either one of them.

XHTML ENTITIES Format:
- Named reference : "&entity_name;" (i.e. an ampersand, the entity name, and then a semi-colon).
- Numeric reference:
- a. Decimal reference : "&#decimal_code;" (i.e. an ampersand, a hash symbol (which signals that a number reference is coming), the character's number, and then a semi colon)
- b. Hex reference     : "&#xhex-code;" (i.e. an ampersand, a hash symbol (which signals that a number reference is coming), x which indicates the character's number is in hex, and then a semi colon)


~~~{.cpp}
// C++
field.SetProperty( TextLabel::Property::TEXT, "Named Entity: &amp;  Numeric Entity: Decimal Entity: &#9827;  Hex Entity: &#x2660;" );
~~~

![ ](XHTML_entity.png)

## SPECIAL CHARACTERS HANDLING IN MARKUP

Three special characters are supported :
- < : Less Than. It means beginning of tag.
- > : Greater Than. It means end of tag.
- & : Ampersand. It means beginning of XHTML Entity.

> "&" usage in markup style changed from Tizen 4.0.
"To display special character needs as regular, prepend it with two backslashes in the string."

Below are some examples

~~~{.cpp}
// C++ ( Wrong usage to print text "Testing of < special character" )
field.SetProperty( TextLabel::Property::TEXT, "Testing of < special character" );
~~~

![ ](SpecialCharacter1.png)

~~~{.cpp}
// C++ ( Wrong usage to print text "Testing of & special character" )
field.SetProperty( TextLabel::Property::TEXT, "Testing of & special character" );
~~~

![ ](SpecialCharacter1.png)

~~~{.cpp}
// C++ ( Correct usage to print text "Testing of & < > special characters" )
field.SetProperty( TextLabel::Property::TEXT, "Testing of \\& \\< \\> special characters" );
~~~

![ ](SpecialCharacters.png)

*/
