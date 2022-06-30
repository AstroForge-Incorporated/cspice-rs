use crate::time::julian_date::JulianDate;
use crate::time::{Calendar, Gregorian, Scale, ET, UTC};
use crate::SPICE;
use std::fmt::{Display, Formatter};
use std::marker::PhantomData;

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct DateTime<T: Calendar, S: Scale> {
    pub year: i16,
    pub month: u8,
    pub day: u8,
    pub hour: u8,
    pub minute: u8,
    pub second: f32,
    /// Number of seconds offset
    pub zone: i32,
    calendar: PhantomData<T>,
    scale: PhantomData<S>,
}

impl<C: Calendar, S: Scale> DateTime<C, S> {
    pub fn new(year: i16, month: u8, day: u8, hour: u8, minute: u8, second: f32) -> Self {
        Self::with_zone(year, month, day, hour, minute, second, 0)
    }

    pub fn with_zone(
        year: i16,
        month: u8,
        day: u8,
        hour: u8,
        minute: u8,
        second: f32,
        zone: i32,
    ) -> Self {
        Self {
            year,
            month,
            day,
            hour,
            minute,
            second,
            zone,
            calendar: Default::default(),
            scale: Default::default(),
        }
    }

    /// Convert a DateTime to Ephemeris Time (TDB)
    #[inline]
    pub fn to_et(&self, spice: SPICE) -> ET {
        let year = if self.year > 0 {
            self.year.to_string()
        } else {
            format!("{} BC", self.year.abs() + 1)
        };
        let date = format!(
            "{year}-{}-{} {}:{}:{} {} {}",
            self.month,
            self.day,
            self.hour,
            self.minute,
            self.second,
            S::name(),
            C::short_name()
        );
        let et = spice.string_to_et(date).unwrap();
        ET(et.0 - self.zone as f64)
    }

    #[inline]
    pub fn to_julian_date(&self, spice: SPICE) -> JulianDate<S> {
        self.to_et(spice).to_julian_date(spice)
    }

    #[inline]
    pub fn from_julian_date(jd: JulianDate<S>, spice: SPICE) -> Self {
        jd.to_et(spice).to_date_time(spice)
    }
}

impl<C: Calendar, S: Scale> Display for DateTime<C, S> {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        let zone_hrs = self.zone as f64 / 3600.0;
        let whole_hrs = zone_hrs.floor() as i32;
        let mins = (zone_hrs.fract() * 60.0) as i32;
        write!(
            f,
            "{}-{}-{} {}:{}:{} {:+}:{mins} {} {}",
            self.year,
            self.month,
            self.day,
            self.hour,
            self.minute,
            self.second,
            whole_hrs,
            S::name(),
            C::short_name()
        )
    }
}

#[cfg(feature = "chrono")]
impl From<chrono::DateTime<chrono::FixedOffset>> for DateTime<Gregorian, UTC> {
    fn from(c: chrono::DateTime<chrono::FixedOffset>) -> Self {
        use chrono::{Datelike, Timelike};
        let seconds = c.second() as f32 + c.nanosecond() as f32 / 1_000_000.0;
        DateTime::with_zone(
            c.year() as i16,
            c.month() as u8,
            c.day() as u8,
            c.hour() as u8,
            c.minute() as u8,
            seconds,
            c.timezone().local_minus_utc(),
        )
    }
}