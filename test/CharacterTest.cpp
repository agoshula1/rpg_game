#define BOOST_TEST_MODULE Character
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
// #include <boost/test/data/monomorphic.hpp>

#include <rpg/Character.hpp>

namespace bdata = boost::unit_test::data;
// namespace utf = boost::unit_test;

namespace std{
    // format printing of Character moves (vector of strings)
    ostream &operator<<(std::ostream &stream, std::vector<std::string> const &moves){
      stream << "[";
      if(!moves.empty()) stream << moves[0];
      for (int i = 1; i < moves.size(); ++i)
          stream << "," << moves[i];
      stream << "]";
      return stream;
    }

    // format printing of Character instance
    ostream &operator<<(std::ostream &stream, Character const &chr) {
      stream << "Name: " << chr.get_name();
      stream << ", HP: " << chr.get_hp();
      
      // print moves
      stream << ", Moves: " << chr.get_moves();
      return stream;
    }
}

std::vector<std::string> data_names = {"Test1", "test!", "_T E S T_"};
std::vector<std::vector<std::string>> data_moves = {{"move1", "move2"}, {}, {"MOVEA"}};

bool moves_same(const std::vector<std::string>& mvs1, const std::vector<std::string>& mvs2){
  if(mvs1.size() != mvs2.size()) return false;
  for(int i = 0; i < mvs1.size(); ++i){
      if(mvs1[i] != mvs2[i]) return false;
  }
  return true;
}

BOOST_AUTO_TEST_SUITE(invalid_arg_exception)

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

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(members_set)

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
    BOOST_TEST(moves_same(test.get_moves(), moves));
  }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(alive_status)

  BOOST_AUTO_TEST_CASE(is_alive){
    Character test = Character("A",3,{});
    BOOST_TEST(test.alive());
  }

  BOOST_AUTO_TEST_CASE(is_not_alive){
    Character test = Character("A",5,{});
    test.apply_hp_effect(-5); // depends on apply_hp_effect() to work properly with negative input
    BOOST_TEST(!test.alive());
  }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(heal_and_damage)
  
  BOOST_AUTO_TEST_CASE(heal){
    Character test1 = Character("A",10,{});
    test1.apply_hp_effect(-7);

    // HP < max after healing
    test1.apply_hp_effect(5);
    BOOST_TEST(test1.get_hp() == 8);

    // HP == max after healing
    test1.apply_hp_effect(2);
    BOOST_TEST(test1.get_hp() == 10);

    // HP remains == max
    test1.apply_hp_effect(1);
    BOOST_TEST(test1.get_hp() == 10);

    Character test2 = Character("B",INT_MAX,{});
    // HP remains == max (without int overflow)
    test2.apply_hp_effect(2);
    BOOST_TEST(test2.get_hp() == INT_MAX);
  }

  BOOST_AUTO_TEST_CASE(damage){
    Character test = Character("A",6,{});

    // HP > 0 after damage taken
    test.apply_hp_effect(-4);
    BOOST_TEST(test.get_hp() == 2);

    // HP = 0 after damage taken
    test.apply_hp_effect(-2);
    BOOST_TEST(test.get_hp() == 0);

    // HP remains = 0
    test.apply_hp_effect(-3);
    BOOST_TEST(test.get_hp() == 0);
  }

  BOOST_AUTO_TEST_CASE(no_effect){
    Character test = Character("A",1,{});

    // HP unchanged by zero effect
    test.apply_hp_effect(0);
    BOOST_TEST(test.get_hp() == 1);

    test.apply_hp_effect(-1);

    // HP not changeable after character is dead
    test.apply_hp_effect(5);
    BOOST_TEST(test.get_hp() == 0);
    test.apply_hp_effect(-5);
    BOOST_TEST(test.get_hp() == 0);
  }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(full_heal)

  BOOST_AUTO_TEST_CASE(heal_to_max){
    Character test = Character("A",7,{});
    test.apply_hp_effect(-4);

    // HP is topped to max
    test.full_heal();
    BOOST_TEST(test.get_hp() == 7);
  }

  BOOST_AUTO_TEST_CASE(no_effect){
    Character test = Character("A",20,{});
    
    // HP remains = max
    test.full_heal();
    BOOST_TEST(test.get_hp() == 20);

    test.apply_hp_effect(-20);
    // HP not changeable after character is dead
    test.full_heal();
    BOOST_TEST(test.get_hp() == 0);
  }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(add_moves)
  
  BOOST_AUTO_TEST_CASE(to_empty){
    Character test = Character("A",3,{});

    // added to empty list of moves
    std::vector<std::string> new_moves = {"move1", "move2", "move3"};
    test.add_moves(new_moves);
    BOOST_TEST(moves_same(test.get_moves(), new_moves));
  }

  BOOST_AUTO_TEST_CASE(to_existing){
    Character test = Character("A",3,{"move1","move2"});

    // added to already populated list of moves
    test.add_moves({"move3", "move4"});
    BOOST_TEST(moves_same(test.get_moves(), {"move1","move2","move3","move4"}));
  }

BOOST_AUTO_TEST_SUITE_END()