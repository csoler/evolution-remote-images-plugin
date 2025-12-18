# evolution-remote-images-plugin
Plugin to add a "paste image link" to the message composer.

## Dependencies

Compilation requires the following packages: 
Execution requires the following packages: [none]

## Compilation with cmake

<pre>
 $ mkdir build
 $ cd build
 $ cmake -DCMAKE_INSTALL_PREFIX=~/.local/share/evolution/modules -DFORCE_INSTALL_PREFIX=ON ..
</pre>

## Compilation with qmake

<pre>
 $ qmake -r CONFIG=debug
 $ make
 $ make install
</pre>

## TODO

* add a button in the messsage composer to form the link
* have a valgrind pass
