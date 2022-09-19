#ifndef TIMER_H
#define TIMER_H

/**
 * Class to count time intervals. It can act as a stopwatch, or
 * simply to retrieve the system time.
 *
 * \author Giovane Kuhn (grkuhn at gmail dot com)
 * \date 2007-03-28 Class created
 * \date 2008-03-31 Several modifications by Leonardo Garcia Fischer
 */
class Timer {
    public:

        /**
			Creates a new timer. 
		*/
        Timer();

		/**
			Timer destructor. For now, it does nothing.
		*/
        virtual ~Timer();

        /**
			Start the time counting.
		*/
        void start();

        /**
			Stop this timer. The time elapsed beetween the last call
			to start() is acumulated in the elapsed time.
		*/
        void stop();

		/**
			Reset the elapsed time to 0. Does not change the 
			counting state of this clock (it does not call the
			stop() method.
		*/
		void reset();

        /**
			Returns the elapsed time.

			\return If the timer is stoped, returns the acumulated
			time beetween all calls to start() and stop() methods, 
			otherwise returns the acumulated time plus the time beetween
			the last call to start() and now.
		*/
        double elapsed() const;

		/**
			Returns the current system time. The timer state is
			not affected by this method.
		*/
        double getTime() const;

    private:

		/** The time that that the last call to start() was done. */
        double startTime;

		/** The elapsed time beetween all calls to start() and stop(). */
		double timeElapsed;

		/** Informs that the clock is counting time or not. */
		bool stoped;
};

#endif // TIMER_H
