#include "pch.h"
#include "common/level/macro_level_script_builder.cpp"

#include <initializer_list>

#include "common/level/level_script_part.cpp"
#include "common/level/util.cpp"
#include "common/script/i_level_script_builder.cpp"
#include "gmock/gmock.h"

// TODO: Move this to a common file.
typedef uintptr_t LevelScript;

void expect_scripts(const MacroLevelScriptBuilder& builder,
                    std::initializer_list<LevelScript> expected_scripts) {
  const auto entry_pointer = builder.get_entry_pointer();
  const auto size = builder.size();

  const std::vector<LevelScript> actual_scripts(
      entry_pointer,
      entry_pointer + size);

  EXPECT_THAT(actual_scripts, testing::ElementsAreArray(expected_scripts));
}

TEST(MacroLevelScriptBuilderTest, ReturnsEmptyByDefault) {
  MacroLevelScriptBuilder builder;
  expect_scripts(builder, {});
}

TEST(MacroLevelScriptBuilderTest, CanAddScript) {
  MacroLevelScriptBuilder builder;

  builder.add_script(1)
         .add_script(2)
         .add_script(3)
         .add_script(4);

  expect_scripts(builder, {1, 2, 3, 4});
}

TEST(MacroLevelScriptBuilderTest, CanAddScriptsWithInitializerList) {
  MacroLevelScriptBuilder builder;

  builder.add_scripts({1, 2})
         .add_scripts({3, 4});

  expect_scripts(builder, {1, 2, 3, 4});
}

TEST(MacroLevelScriptBuilderTest, CanAddScriptsWithPointer) {
  MacroLevelScriptBuilder builder;

  LevelScript part1[] = {1, 2};
  LevelScript part2[] = {3, 4};
  builder.add_scripts(part1, 2)
         .add_scripts(part2, 2);

  expect_scripts(builder, {1, 2, 3, 4});
}

TEST(MacroLevelScriptBuilderTest, CanAddBuilder) {
  MacroLevelScriptBuilder builder;

  const auto child1 = std::make_shared<MacroLevelScriptBuilder>();
  const auto child2 = std::make_shared<MacroLevelScriptBuilder>();

  builder.add_builder(child1);
  (*child1).add_scripts({1, 2})
           .add_builder(child2);
  child2->add_scripts({3, 4});

  expect_scripts(builder, {1, 2, 3, 4});
}
