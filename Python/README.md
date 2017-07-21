<!-- TOC -->

- [Moduels](#moduels)
    - [Import a File from a Subdirectory](#import-a-file-from-a-subdirectory)
- [Typical Technique](#typical-technique)
- [Language Comparison](#language-comparison)
    - [Data Manipulation](#data-manipulation)
- [access indivisual char in a string](#access-indivisual-char-in-a-string)
- [Methods to access indivisual char](#methods-to-access-indivisual-char)
- [access indivisual element](#access-indivisual-element)
- [slice/sublist](#slicesublist)
- [list comprehension](#list-comprehension)
- [slice/sublist](#slicesublist-1)
    - [Control Flow](#control-flow)
    - [Function](#function)
- ['_' means ignoring that returned value](#_-means-ignoring-that-returned-value)
    - [File I/O](#file-io)
- [Or use variable, e.g., $inFile, instead of INFILE](#or-use-variable-eg-infile-instead-of-infile)
- [Read the whole file](#read-the-whole-file)
- [read a line](#read-a-line)
- [array environment: read the whole file to an array](#array-environment-read-the-whole-file-to-an-array)
- [scale environment: one file at a time](#scale-environment-one-file-at-a-time)

<!-- /TOC -->

Python-HOWTO

**google** 'markdown editor github'

```python
import matplotlib
def proc:
    print "Hello World"
```

# Moduels

## Import a File from a Subdirectory

**Steps**:
* Create a subdirectory named `lib`
* Create an empty file  named `lib\__init__.py`
* Create a Python file `lib\foo.py`
* Import `foo.py`

```python
from lib import foo
#or
import lib.foo as foo
```

# Typical Technique

```python
# create an anonymous class & then its object
args = type('', (), {})()
# then add attributes to it
args.x = 10
``` 

# Language Comparison

## Data Manipulation

<table>
  <tr>
    <td></td><td><b>Python</b></td><td><b>Perl</b></td>
  </tr>
  <tr>
    <td><b>String</b></td>
    <td>
     A string is an array.
    <pre lang="python">
# access indivisual char in a string
str[2]
    </pre>
    </td>
    <td>
    A string is NOT an array
    <pre lang="perl">
# Methods to access indivisual char
my @chars = split("", $string);
my $char = substr($string, 7, 1);
    </pre>
    </td>        
  </tr>

  <tr>
    <td>
    Python: <b>List</b><br>       
    Perl: <b>Array</b>
    </td>
    <td>
    <pre lang="python">
t = (1,2, 3)   # tuple
a = [1, 2, 3]  # list
xrange(3)    # 0, 1, 2 not including 3
range(3)
# access indivisual element
a[2]
# slice/sublist
a[1:3]  # 1, 2 NOT including 3
a[:3]   # 0, 1, 2
a.append(2)
# list comprehension
[(i+1, a[0]) for i, a in enumerate(lst)],
    </pre>
    </td>
    <td>
    <pre lang="perl">
@a=(1, 2, 3)
$r=[0..3] # reference to annonymous array    

@a=(0..3) # 0, 1, 2, 3 including 3
#slice/sublist
@a[1..3] # $a[1], $a[2], $a[3]
push @a, 10  # push to the end
$e= pop @a
unshift @a, 10 # add to the start
$e = shift @a
    </pre>
    </td>     
  </tr>  

  <tr>
    <td><b>Dict</b></td>
    <td>
    <pre lang="python">
d = {"k1": 1} # like Json

d["k1"]
    </pre>
    </td>
    <td>
    <pre lang="perl">
%d = ("k1" => 1)  
$r = {"k1" => 1} # ref to annonymous hash
%d = ("k1" , 1)  # list syntax

$d{"k1"} # or $d{k1}
    </pre>
    </td>     
  </tr>    
</table>

## Control Flow

<table>
  <tr>
    <td></td><td><b>Python</b></td><td><b>Perl</b></td>
  </tr>

  <tr>
    <td><b>Branch</b></td>
    <td>
<pre lang="python">
if expr:
  #...
elif expr2:
  #...
x=y if y is not None else ''
</pre>    
    </td>
    <td>
<pre lang="python">
if expr:
  #...
elsif expr2:
</pre>        
    </td>    
  </tr>

  <tr>
    <td><b>Loop List</b></td>
    <td>
    <pre lang="python">
for i in xrange(10):

for idx, elem in enumerate(list):    
    </pre>
    </td>
    <td>
    <pre lang="perl">
foreach my $TestIdx( @test_list )

for my \$i (0 .. $#seqStrings)
    </pre>
    </td>
  </tr>
  <tr>
    <td><b>Loop Dict</b></td>
    <td>
    <pre lang="python">
for key in d:

\# Python 2.x
for key, value in d.iteritems():
\# Python 3.x
for key, value in d.items(): 
    </pre>
    </td>
    <td>
    <pre lang="perl">
for my $seqString (keys %seq_list)
    </pre>
    </td>
  </tr>
<table>

## Function

<table>
  <tr>
    <td></td><td><b>Python</b></td><td><b>Perl</b></td>
  </tr>
  <tr>
    <td><b>Function: Def</b></td>
    <td>
    <pre lang="python">
def foo(arg):
    # return a list
    return (v1, v2)
    </pre>
    </td>
    <td>
    <pre lang="perl">
sub foo; # declare it

sub foo { # argument in @_
    my ($a1, $a2) = shift;
    return ($v1, $2);
}
    </pre>
    </td>        
  </tr>

  <tr>
    <td><b>Function: Call</b></td>
    <td>
    <pre lang="python">
# '_' means ignoring that returned value    
v1, _ = foo(arg)
    </pre>
    </td>
    <td>
    <pre lang="perl">
($v1, $v2) = foo();
    </pre>
    </td>        
  </tr>  
</table> 

## File I/O

<table>
  <tr>
    <td></td><td><b>Python</b></td><td><b>Perl</b></td>
  </tr>
  <tr>
    <td><b>Open/Close File</b></td>
    <td>
    <pre lang="python">
try:
    with open(fileName, 'wt') as fout:
        #...
except IOError:
    print( 'FAILED to open {}'.format( fileName ) )    
    </pre>
    </td>
    <td>
    <pre lang="perl">    
# Or use variable, e.g., $inFile, instead of INFILE
open INFILE, "<$gOutput";
close INFILE;
    </pre>
    </td>
  </tr>
  <tr>
    <td><b>Read/Write File</b></td>
    <td>
    <pre lang="python">
# Read the whole file
text = fin.read() # to a string
lines = fin.readlines() # to a list
lines = list(fin)
# read a line
line = fin.readline() # read a line
while line: # per-line processing

for line in fin:
    </pre>
    </td>
    <td>
    <pre lang="perl">    
# array environment: read the whole file to an array
@data = <INFILE>;
# scale environment: one file at a time
while ( $line = <INFILE> )
    </pre>
    </td>
  </tr>  
</table>


<table>
  <tr>
    <td></td><td><b>Python</b></td><td><b>Perl</b></td>
  </tr>
  <tr>
    <td><b>String</b></td>
    <td>
    <pre lang="python">
    </pre>
    </td>
    <td>
    <pre lang="perl">
    </pre>
    </td>        
  </tr>
</table> 
