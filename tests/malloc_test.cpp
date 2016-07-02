#include "../src/malloc.hpp"
#include "../lib/fff/fff.h"
#include <gtest/gtest.h>

DEFINE_FFF_GLOBALS

extern "C" void* __real_malloc (size_t size);
FAKE_VALUE_FUNC1(void *, __wrap_malloc, size_t)

extern "C" void __real_free (void *ptr);
FAKE_VOID_FUNC1(__wrap_free, void *)

FAKE_VOID_FUNC0(mock_call_malloc)

class CallMallocTest : public testing::Test
{
protected:
  virtual void SetUp ();
  virtual void TearDown ();
};

void CallMallocTest::SetUp ()
{
  RESET_FAKE(mock_call_malloc);
  RESET_FAKE(__wrap_malloc);
  RESET_FAKE(__wrap_free);
  FFF_RESET_HISTORY();
}

void CallMallocTest::TearDown ()
{
  // Not doing this cause the test to finish with segfault, since there are
  // others depending on the real malloc and free
  __wrap_malloc_fake.custom_fake = __real_malloc;
  __wrap_free_fake.custom_fake = __real_free;
}

TEST_F (CallMallocTest, GivenCallMalloc_WithMockedMallocAndFree_CallHistoryShouldExist) {
    // GIVEN
    mock_call_malloc_fake.custom_fake = call_malloc;
    __wrap_malloc_fake.return_val = (void *) 0xffff;

    // WHEN
    mock_call_malloc();

    // THEN
    ASSERT_EQ(fff.call_history[0], (void *)mock_call_malloc);
    ASSERT_EQ(fff.call_history[1], (void *)__wrap_malloc);
    ASSERT_EQ(fff.call_history[2], (void *)__wrap_free);
}

TEST_F (CallMallocTest, GivenCallMalloc_WithRealMallocAndFree_CallHistoryShouldExist) {
    // GIVEN
    mock_call_malloc_fake.custom_fake = call_malloc;
    __wrap_malloc_fake.custom_fake = __real_malloc;
    __wrap_free_fake.custom_fake = __real_free;

    // WHEN
    mock_call_malloc();

    // THEN
    ASSERT_EQ(fff.call_history[0], (void *)mock_call_malloc);
    ASSERT_EQ(fff.call_history[1], (void *)__wrap_malloc);
    ASSERT_EQ(fff.call_history[2], (void *)__wrap_free);
}
