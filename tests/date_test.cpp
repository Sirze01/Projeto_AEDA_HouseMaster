#include <gtest/gtest.h>
#include "../src/Services.h"

using testing::Eq;

TEST(Date, leap_year){
    EXPECT_EQ(date(1,2,2020,20,00).getDaysInMonth(), 29);
    date(1,2,2020,20,00).getDaysInMonth();

    EXPECT_EQ(date(1,2,2021,20,00).getDaysInMonth(), 28);
    date(1,2,2020,20,00).getDaysInMonth();
}

TEST(Date, month_days){
    EXPECT_EQ(date(1,1,2020,00,00).getDaysInMonth(), 31);


    EXPECT_EQ(date(1,3,2020,00,00).getDaysInMonth(), 31);


    EXPECT_EQ(date(1,4,2020,00,00).getDaysInMonth(), 30);


    EXPECT_EQ(date(1,5,2020,00,00).getDaysInMonth(), 31);


    EXPECT_EQ(date(1,6,2020,00,00).getDaysInMonth(), 30);


    EXPECT_EQ(date(1,7,2020,00,00).getDaysInMonth(), 31);


    EXPECT_EQ(date(1,8,2020,00,00).getDaysInMonth(), 31);


    EXPECT_EQ(date(1,9,2020,00,00).getDaysInMonth(), 30);

    EXPECT_EQ(date(1,10,2020,00,00).getDaysInMonth(), 31);


    EXPECT_EQ(date(1,11,2020,00,00).getDaysInMonth(), 30);


    EXPECT_EQ(date(1,12,2020,00,00).getDaysInMonth(), 31);

}

TEST(Date, valid_date){
    date val2;
    val2.day = 25;
    val2.month = 06;
    val2.year = 2020;
    val2.hours = 9;
    val2.minutes = 0;
    EXPECT_EQ(date(25,06,2020, 9,00), val2);

    date val1;
    val1.setDate(25,06,2020,9,0);
    EXPECT_EQ(val1, val2);

    EXPECT_THROW(date d1(31, 2, 2020, 20,0), date::InvalidDate);

    EXPECT_THROW(date d1(31, 4, 2020, 20,0), date::InvalidDate);

    EXPECT_THROW(date d1(30, 13, 2020, 20,0), date::InvalidDate);

    EXPECT_THROW(date d1(32, 1, 2020, 24,0), date::InvalidDate);

    EXPECT_THROW(date d1(32, 1, 2020, 20,60), date::InvalidDate);
}

TEST(DATE, print_date){
    EXPECT_EQ(date(12, 12, 2012, 12, 51).dateToStr(), "12/12/2012  12:51");
}