#include <boost/python.hpp>
#include "debug.h"

BOOST_PYTHON_MODULE(debug_module)
{
    using namespace boost::python;
    def("_gl_engine_handle_error", _gl_engine_handle_error);
    def("_gl_engine_info", _gl_engine_info);
    def("_gl_engine_warn", _gl_engine_warn);
}

