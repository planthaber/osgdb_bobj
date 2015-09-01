#include <boost/test/unit_test.hpp>
#include <plugin_bobj/Dummy.hpp>

using namespace plugin_bobj;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    plugin_bobj::DummyClass dummy;
    dummy.welcome();
}
