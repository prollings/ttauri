# Binary Unicode Data Format
This is a data format that is read by the UnicodeData class and is formed from
several Unicode ucd data files.

All integers are in little endian format.
All messages are aligned to 64 bit.

By merging fields inside 32 and 64 bit integer integers, we can compress the data by about 1/3.

## Header

 | Type     | Name                    | Description                                                              | 
 | -------- | ----------------------- | ------------------------------------------------------------------------ |
 | uint32_t | magic                   | Identifier of this file format 'bucd' (0x62756364)                       |
 | uint32_t | version                 | Version of the file format 0x1                                           |
 | uint32_t | nrDescriptions          | Number of descriptions that follow the header                  |
 | uint32_t | nrCompositions          | Number of compositions that follow the code unit descriptions  |

## Description
The Unicode code-point descriptions are orded by increasing code-point value.

 | Type     | Bits  | Name                     | Description                                                              | 
 | -------- | ----- | ------------------------ | ------------------------------------------------------------------------ |
 | uint64_t | 63:43 | codePoint                | The code unit descriptions are orded by increasing code-point value      |
 |          | 42:39 | bidirectionalClass       | 15 enum values, listed below                                                                    |
 |          | 38:35 | graphemeUnitType         | 15 enum values, listed below                                             |
 |          | 34    | canonicalDecomposition   | Canonical decomposition flag                                             |
 |          | 33:26 | decompositionOrder       | Orderering value for sorting decomposition code-points                   ||
 |          | 25:21 | decompositionLength      | Number of decomposition code points, maximum 18                          |
 |          | 20:0  | decompositionOffset /    | decompositionLength >= 2 byte offset * 8 in the file.                    |
 |          |       | decompositionCodePoint / | decompositionLength == 1 then this contains the code-point               |
 |          |       | reserved                 | decompositionLength == 0                                                 |
 
## Composition
The compositions are ordered by increasing start code-point followed by increasing composing
code-point value.

For space saving and cache locality, the decompositionOffset of the code unit descriptions may point inside this table.

 | Type     | Bits  | Name                    | Description                                                              | 
 | -------- | ----- | ----------------------- | ------------------------------------------------------------------------ |
 | uint64_t | 63:43 | startCharacter          | The code point of the first character of a pair                          |
 |          | 42:22 | composingCharacter      | The code point of the second character of a pair                         |
 |          | 21    | reserved                | 0                                                                        |
 |          | 20:0  | composedCharacter       | The code point resulting of the combining of the pair                    |

## Other decompositions
The decompositionOffset can point into the Composition table for decompositions of two code-points.
For other decompositions it will need to point in the following table.

 | Type     | Bits  | Name                    |
 | -------- | ----- | ----------------------- |
 | uint64_t | 63:43 | 1st code point          |
 |          | 42:22 | 2nd code point          |
 |          | 21    | reserved                |
 |          | 20:0  | 3rd code point          |
 | uint64_t | 63:43 | 4th code point          |
 |          | 42:22 | 5th code point          |
 |          | 21    | reserved                |
 |          | 20:0  | 6th code point          |
 | uint64_t |       | etc...                  |


## Grapheme unit types

 | Name                  | Value |
 | --------------------- | -----:|
 | Other                 |     0 |
 | CR                    |     1 |
 | LF                    |     2 |
 | Control               |     3 |
 | Extend                |     4 |
 | ZWJ                   |     5 |
 | Regional_Indicator    |     6 |
 | Prepend               |     7 |
 | SpacingMark           |     8 |
 | L                     |     9 |
 | V                     |    10 |
 | T                     |    11 |
 | LV                    |    12 |
 | LVT                   |    13 |
 | Extended Pictographic |    14 |

## Bidirectional Class

 | Name    | Description             | Value | Derived |
 | ------- | ----------------------- | -----:| ------- |
 | Unknown |                         |     0 |         |
 | LRE     | Left to right embedding |     0 |         |
 | RLE     | Right to left embedding |     0 |         |
 | LRO     | Left to right override  |     0 |         |
 | RLO     | Right to left override  |     0 |         |
 | LRI     | Left to right isolate   |     0 |         |
 | RLI     | Right to left isolate   |     0 |         |
 | FSI     | First strong isolate    |     0 |         |
 | PDI     | Pop directional isolate |     0 |         |
 | PDF     | Pop directional format  |     0 |         |
 | L       | Left-to-right           |     1 | Letter  |
 | R       | Right-to-left           |     2 | Letter  |
 | AL      | Arabic Letter           |     3 | Letter  |
 | EN      | European Number         |     4 | Digit   |
 | ES      | European Separator      |     5 |         |
 | ET      | European Terminator     |     6 |         |
 | AN      | Arabic Number           |     7 | Digit   |
 | CS      | Common Separator        |     8 |         |
 | NSM     | Nonspacing Mark         |     9 |         |
 | BN      | Boundary Neutral        |    10 |         |
 | B       | Paragraph Separator     |    11 |         |
 | S       | Segment Separator       |    12 |         |
 | WS      | White space             |    13 |         |
 | ON      | Other Neutral           |    14 |         |
