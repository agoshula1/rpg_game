#define BOOST_TEST_MODULE Character
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
// #include <boost/test/data/monomorphic.hpp>

#include <rpg/Character.hpp>

namespace bdata = boost::unit_test::data;

namespace std{
    ostream &operator<<(std::ostream &stream, std::vector<std::string> const &moves){
      stream << "[";
      if(!moves.empty()) stream << moves[0];
      for (int i = 1; i < moves.size(); ++i)
          stream << "," << moves[i];
      stream << "]";
      return stream;
    }

    ostream &operator<<(std::ostream &stream, Character const &chr) {
      stream << "Name: " << chr.get_name(); // print name
      stream << ", HP: " << chr.get_hp(); // print hp
      
      // print moves
      // const std::vector<std::string> moves = chr.get_moves();
      stream << ", Moves: " << chr.get_moves();
      return stream;
    }
}

std::vector<std::string> data_names = {"Test1", "test!", "_T E S T_"};
std::vector<std::vector<std::string>> data_moves = {{"move1", "move2"}, {}, {"MOVEA"}};

BOOST_AUTO_TEST_CASE(invalid_name){
    // exception expected for empty name
    BOOST_CHECK_THROW(Character("",1,{}), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(invalid_negative_hp){
    // exception expected for negative hp
    BOOST_CHECK_THROW(Character("A",-2,{}), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(invalid_zero_hp){
    // exception expected for zero hp
    BOOST_CHECK_THROW(Character("B",0,{}), std::invalid_argument);
}

BOOST_DATA_TEST_CASE(
  name_set,
  bdata::make(data_names) ^ bdata::xrange(1,4) ^ bdata::make(data_moves),
  name, hp, moves)
{
  Character test = Character(name,hp,moves);
  BOOST_TEST(test.get_name() == name); // initialized with input name
}

BOOST_DATA_TEST_CASE(
  hp_set,
  bdata::make(data_names) ^ (bdata::make(INT_MAX) + bdata::random(1,100)) ^ bdata::make(data_moves),
  name, hp, moves)
{
  Character test = Character(name,hp,moves);
  BOOST_TEST(test.get_hp() == hp); // initialized with input hp
}

BOOST_DATA_TEST_CASE(
  moves_set,
  bdata::make(data_names) ^ bdata::xrange(1,4) ^ bdata::make(data_moves),
  name, hp, moves)
{
  Character test = Character(name,hp,moves);
  // initialized with input moves
  std::vector<std::string> set_moves = test.get_moves();
  BOOST_TEST(set_moves.size() == moves.size());
  for(int i = 0; i < moves.size(); ++i){
      BOOST_TEST(set_moves[i] == moves[i]);
  }
}