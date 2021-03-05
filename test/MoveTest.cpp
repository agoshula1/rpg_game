#define BOOST_TEST_MODULE Move
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <climits>

#include <rpg/Move.hpp>

namespace bdata = boost::unit_test::data;

namespace std{
    // format printing of Move instance
    ostream &operator<<(std::ostream &stream, Move const &mv){
      stream << "Name: " << mv.get_name();
      stream << ", Description: " << mv.describe();
      stream << ", HP Effect: " << mv.get_hp_effect();
      stream << ", Targets all: " << mv.targets_all();
      return stream;
    }
}

std::vector<std::string> data_names = {"Test1", "test!", "_T E S T_"};
std::vector<std::string> data_descrips = {"A Description", "another move.", "L_A_S_T!"};
std::vector<int> data_effects = {INT_MIN, 0, INT_MAX};
std::vector<bool> data_target = {true, false, false};

BOOST_AUTO_TEST_SUITE(invalid_arg_exception)

  BOOST_AUTO_TEST_CASE(invalid_name){
      // exception expected for empty name
      BOOST_CHECK_THROW(Move("","description",0,false), std::invalid_argument);
  }

  BOOST_AUTO_TEST_CASE(invalid_description){
      // exception expected for empty description
      BOOST_CHECK_THROW(Move("name","",0,false), std::invalid_argument);
  }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(members_set)

  BOOST_DATA_TEST_CASE(
    name_set,
    bdata::make(data_names) ^ bdata::make(data_descrips) ^ bdata::random(1,100) ^ bdata::make(false),
    name, description, hp_effect, target_all)
  {
    Move test = Move(name,description,hp_effect,target_all);
    BOOST_TEST(test.get_name() == name); // initialized with input name
  }

  BOOST_DATA_TEST_CASE(
    description_set,
    bdata::make(data_names) ^ bdata::make(data_descrips) ^ bdata::random(1,100) ^ bdata::make(true),
    name, description, hp_effect, target_all)
  {
    Move test = Move(name,description,hp_effect,target_all);
    BOOST_TEST(test.describe() == description); // initialized with input description
  }

  BOOST_DATA_TEST_CASE(
    hp_effect_set,
    bdata::make(data_names) ^ bdata::make(data_descrips) ^ bdata::make(data_effects) ^ bdata::make(false),
    name, description, hp_effect, target_all)
  {
    Move test = Move(name,description,hp_effect,target_all);
    BOOST_TEST(test.get_hp_effect() == hp_effect); // initialized with input hp effect
  }

  BOOST_DATA_TEST_CASE(
    target_all_set,
    bdata::make(data_names) ^ bdata::make(data_descrips) ^ bdata::random(1,100) ^ bdata::make(data_target),
    name, description, hp_effect, target_all)
  {
    Move test = Move(name,description,hp_effect,target_all);
    BOOST_TEST(test.targets_all() == target_all); // initialized with input boolean to indicate if targets all
  }

BOOST_AUTO_TEST_SUITE_END()