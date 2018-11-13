# Gray-Scott simulator
Reaction Diffusion simulation of Gray-Scott model.
Gray-Scott equations are partial differential equations that model the time evolution of three reacting and diffusing chemical species. 
Their ability to generate beautiful patterns that resemble naturally observed patterns makes them interesting. 

[More about Gray-Scott model](https://groups.csail.mit.edu/mac/projects/amorphous/GrayScott/)

## Typical patterns 
![Pattern 1](/samples/pattern-1.png)
![Pattern 2](/samples/pattern-2.png)
![Pattern 3](/samples/pattern-3.png)

Parameters used to generate the above patterns
U_DIFF = 0.2 
V_DIFF = 0.1 
SIZE = 256
DT = 1

1) FEED = 0.049, KILL = 0.06
2) FEED = 0.048, KILL = 0.066
3) FEED = 0.042, KILL = 0.062

## Dependencies
* [PNGWriter](https://github.com/pngwriter/pngwriter) : to generate PNG output
  * [zlib](https://github.com/madler/zlib/) 
  * [libpng](http://download.sourceforge.net/libpng)
  * [freetype](http://download.savannah.gnu.org/releases/freetype)

* [Armadillo](http://arma.sourceforge.net/) : to perform matrix operations

## Compilation
After installing the dependincies, just run `make`

---
PS:  This code is a work in progress and is written only for educational purpose.
