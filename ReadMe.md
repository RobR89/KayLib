  This is my code library.  I have created many of these files over many years and now I'm making then available to whoever wants to use them.
This library is licensed under Apache Version 2.0 as can be found here http://www.apache.org/licenses/LICENSE-2.0

```
/*
 * Copyright 2016 Robert Reinhart.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
```

* What is KayLib?  
  KayLib is a collection of files that I have been putting together for several years.  I finally decided to share them.

* Why "KayLib"?  
  Because "O(**K**), this is my (**Lib**)rary" shortened to "KLib".  So why not "KayLib"

* Why are some class names prefixed with a K?  
  I have done this to prevent confusion with commonly used class names.  Originally I was going to do this with all the classes but decided not to.

* What compiler is needed?  
  The library uses c++11 features.  So at least a c++11 compiler is needed.

* Is the library thread safe?  
  Some classes are.  Others are not!  The reason is that it would be impractical and cause serious performance issues.
  Thread safety is mostly just implemented where threading is most likely to occur.

* Use of the boost library.  
  Currently there is only one class that uses boost.  The XML parser.  Some day I hope to change that.
  If you do not want to use boost and have no need of the XML parser you can simply remove them from the project.

#### The Files...
* Graphics/KColor.h  
  This is a generic color container for r,g,b,a data.  It also provides the access to the HTML defined named colors.

* IO/Event.h  
  This is an event dispatcher / Listener setup.  It can use multi-threading to dispatch the messages in a new thread.

* IO/Exceptions.h  
  A collection of Exceptions that I have used in the past.

* IO/KFile.h  
  A collection of functions for working with files and searching and directories.

* IO/KThread.h  
  A small bit of multi-threading support.

* KMath/KMath.h  
  A few predefined values and structures.  Also, includes all other KMath headers.

* KMath/*.h  
  A collection of various math objects.  
  Including:
    * and perlin noise, interpolation, splines, and spherical coordinate objects.

* Parser/JSON.h  
  A small JSON parser.

* Parser/StringParser.h  
  A string parser capable of retrieving data in a variety of ways.  
  Also, UTF versions that are capable of reading UTF8 or UTF16 encoded strings.

* Parser/XMLDocument.h  
  A wrapper for simplifying the boost xml parser.  Will someday be re-written to not use boost.

* String/KString.h  
  A few string useful manipulation functions.

* String/KUTF.h  
  Some useful UTF string manipulation functions.

* Utility/DataCode.h  
  A class for creating .cpp and .h files that contain binary data in unsigned char arrays.  
  Useful for embedding things such as images or other resources in a program.

####Dependancies
  libsdl2-dev
  libsdl2-image-dev
  libsdl2-ttf-dev
  libmysqlclient-dev
  libsqlite3-dev
  libboost-dev
  liblua5.2-dev
  libglm-dev
