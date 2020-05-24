
# INPUT

## 1. Platform
Software: linux-3.18 <br>
Hardware: Gpt Soc 8300 <br>

## 2. Description
      This is virtual input project that simulate keyboard or mouse device,
    it is used for testing...

## 3. Compile input project
    $ make

## 4. Release
    $ make release
    $ ls *.tar.gz
      virtkey-utils-c15a7d5-linux.tar.gz

## 5. Input Usage

### 1> install driver
      $ sudo insmod driver/virtkey.ko

### 2> virkey is input tools, for more infomation, please see virkey's help
      $ tools/virkey  --help
        Usage: virtkey cmd [OPERAND]...
        virtual keyboard program
        Command:
             -k keycode		 Write key code normal
             -s keycode		 Write key code with shift
             -c keycode		 Write key code with ctl
             -a keycode		 Write key code with alt
             -r        		 mouse right click
             -l        		 mouse left  click
             -m        		 mouse middle click
             -x xposion		 mouse move x position
             -y yposion		 mouse move y position
             -v        		 show the keyboard map
       example:
        $ ./tools/virkey -v        // show keyboard map
        $ ./tools/virkey -k 63     // press f5
        $ ./tools/virkey -k 32     // press 'd'
        $ ./tools/virkey -k 28     // press entry
        $ ./tools/virkey -s 24     // press 'O'
