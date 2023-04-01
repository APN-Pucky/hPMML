*******
Install
*******

===========
Linux / Mac
===========

.. code-block:: bash

    git clone https://github.com/AmadeusITGroup/hPMML.git && cd hPMML && ./install.sh

**Usage**

.. code-block:: cpp

    // your_program.cc

    #include <iostream>
    #include "hPMML.h"

    int main() {
        std::cout << "hPMML version: " << hpmml::version << std::endl;
        return 0;
    }

.. code-block:: bash

    g++ your_program.cc -lhPMML

**Requirements**

- Git
- Compiler supporting C++11.
- CMAKE version >= 3.5.1.

=======
Windows
=======

.. code-block:: bat

    git clone https://github.com/AmadeusITGroup/hPMML.git && cd hPMML && install.bat

The compiled library files will be placed under hpmml/hpmml_win

**Usage**

.. code-block:: cpp

    // your_program.cc

    #include <iostream>
    #include "hPMML.h"

    int main() {
        std::cout << "hPMML version: " << hpmml::version << std::endl;
        return 0;
    }

Supposing you cloned and built hPMML under some path like *C:\\Users\\YourUser\\hpmml*.


.. code-block:: bat

    copy C:\Users\YourUser\hpmml\hpmml_win\* .
    g++ your_program.cc -L. -lhPMML

**Requirements**

- Git
- MinGW-W64 supporting C++11
- CMAKE version >= 3.5.1
