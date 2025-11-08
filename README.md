# mkm-editor

## How to compile and run:
Simply run `make`

## First Test - File Read / Writing Function
Ran
```bash
make
echo -e "alpha\nbeta\r\ngamma" > read.txt
./build/mkm read.txt
```
and it gave me first the 200 bytes of the file sucessfully.

`malloc` allocates memory and returns a pointer to it.
On `calloc` the memory is not initialized.
`fseek` used for setting the file pointer at the specific position in the file.
`ftell` used to obtain the current file position of a stream.

## Second Test - Counting Lines
Added `--number` flag for mkm, which displays the number of lines in the given file.

```bash
printf "file has one line" > ol.txt
./build/mkm --number ol.txt
```
Expected output: `1: file has one line`
```bash
printf "\n" > twoblanklines.txt
./build/mkm --number twoblanklines.txt
```
Expected output: 
```bash
1: 
2: 
```

Combined 13 decimal (0x0D) and 10 decimal (0x0A) which is CR and LF, they are used to mark a line break in a text file.