/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "install_py_dlls.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "scriptobject.hpp"

namespace KBEngine{ 
namespace script{

#if KBE_PLATFORM != PLATFORM_WIN32
bool install_py_dlls(void)
{
	return true;
}
#else
typedef PyObject* (*pyfunc)(void);

extern "C" PyObject* PyInit__socket(void);
extern "C" PyObject* PyInit__ssl(void);
extern "C" PyObject* PyInit__hashlib(void);
extern "C" PyObject* PyInit_select(void);
extern "C" PyObject* PyInit__ctypes(void);
extern "C" PyObject* PyInit__elementtree(void);
extern "C" PyObject* PyInit_unicodedata(void);
extern "C" PyObject* PyInit_pyexpat(void);

pyfunc g_funs[] = {&PyInit_pyexpat, &PyInit__socket, &PyInit__ssl, &PyInit__hashlib, 
&PyInit_select, &PyInit__ctypes, &PyInit__elementtree, &PyInit_unicodedata, NULL};

//-------------------------------------------------------------------------------------
bool install_py_dlls(void)
{
	PyObject *modules = PyImport_GetModuleDict();
	int i = 0;

	while(true)
	{
		if(g_funs[i] == NULL)
			break;

		PyObject * m = (*g_funs[i++])();
		struct PyModuleDef *def;
		def = PyModule_GetDef(m);
		if (!def) {
			PyErr_BadInternalCall();
			return false;
		}

		if (PyDict_SetItemString(modules, def->m_name, m) < 0)
			return false;
	}

	return true;
}

//-------------------------------------------------------------------------------------
#endif
}
}
