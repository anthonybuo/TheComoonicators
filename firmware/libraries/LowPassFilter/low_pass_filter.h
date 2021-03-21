#ifndef _FILTER_H_
#define _FILTER_H_

class LowPassFilter {
  public:
    LowPassFilter(double coefficient) : coefficient_{coefficient} {}

    double filter(double sample) {
      double result;

      if (first_time_) {
        result = sample;
        first_time_ = false;
      } else {
        result = sample * coefficient_ + running_sample_ * (1 - coefficient_);
      }

      running_sample_ = result;
      return result;
    }

    void set_coefficient(double new_coefficient) {
      coefficient_ = new_coefficient;
    }

  private:
    // Filter coefficient from 0 to 1, where 1 is no filtering
    double coefficient_;

    // Filter is based on an accumulation of sample
    double running_sample_; 

    // Special case
    bool first_time_ = true;
};

#endif  // _FILTER_H_

