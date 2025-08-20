# 668 file format

This format was only used in an "STM Player" coded by Tran.
Please note that this is not finished, and only has what I feel confident is correct.
This will be updated as needed.

## Header

### Main song

| location |         type           | information                         |
|:---------|:----------------------:|:------------------------------------|
| 0x0000   |          u8            | tempo byte                          |
| 0x0001   |          u8            | total instruments                   |
| 0x0002   |          u8            | total orders                        |
| 0x0003   |          u8            | total patterns                      |
| 0x0004   |          u16           | location of the first sample        |
| 0x0006   |          u8[]          | order list                          |
| -        |          u16           | reserved?                           |
| -        |  u16[total patterns]   | pattern locations, relative to here |
| -        | smphead[total samples] | sample headers                      |

### Sample headers

| location | type | information              |
|:---------|:----:|:-------------------------|
| 0x0000   | u16  | ??? (always 0)           |
| 0x0002   | u16  | length                   |
| 0x0004   | u16  | loop start (0 = none)    |
| 0x0006   | u16  | loop end (0xFFFF = none) |

samples are in 8-bit unsigned format.

## Patterns

each pattern lay out a set of commands

### Commands

#### 0x00 to 0x03

```text
iiiivvvv Innnnnnn

Iiiii -> instrument
vvvv -> volume
nnnnnnn -> note (formatted like MIDI; (octave*12)+note)
```

#### 0x04

```text
bbbbbbbb -> increment by b rows
```

#### 0x05

```text
pppppppp -> jump to position b
```

#### 0x06

end of pattern/padding byte

## DOCS.TXT

These are directly lifted from the text document that came with the player.

### STM2668

```text
STM2668 <file.STM> <fileout> <tempo>

 file.STM  -  full path and filename of input .STM file...
 fileout   -  any valid DOS filename
 tempo     -  the tempo to convert to, i use a different measure than
              ST, a tempo of 50 in st translates nicely into my tempo
              of 3, 60 into 4 but it sounds a bit slow still...
              but notice there should be only one tempo for the whole
              .STM cuz theres only one tempo for the whole output song...
```

### Technical bits

> the mixing is always at 12048Hz, and itll sound fine even at 8mHz,
> except if the CPU is overburdened, then itll just slow down, but
> not too much, and so far no matter how much stress i put on my CPU
> at its lowest speed, it hasn't yet crashed on me...
>  
> one problem though, on the SBPro the output will be a little lower
> and slower cuz i cant figure out for what reason but (sic) i cant seem
> to get the sampling rate high enough in stereo mode, even though
> its well within the SBPro's limitations... but hey, not my fault
> ok! (sic), i just dont feel like spending $100 on a programmers manual...
> so all the shit on this card up to now ive learned the hard way...
>  
> oh, and the stereo channels on the sbpro are left, right, right, left
>  
> oh also, of course if youre running in V86 mode on a 386, the output
> will not be that good...
