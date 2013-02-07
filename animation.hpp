#pragma once

class cAnimation
{
public:
	enum eAnimationType
	{
		SMOOTH,
		JUMP
	};
	enum eAnimationDirection
	{
		PERTURB,
		RESTORE,
		CYCLE,
		PERTURB_REPEAT,
		RESTORE_REPEAT
	};
	cAnimation(const char* name):mName(name),mTimeElapsed(0.0f), mDuration(0.0f){};
	virtual ~cAnimation(){};
	void reset()
	{
		mTimeElapsed = 0.0f;
	}

	inline void Update(float elapsed_time)
	{
		mTimeElapsed += elapsed_time;
		this->DoFrame();
	}
	virtual void DoFrame() = 0;

	//if an animation has a duration < 0.0 it never finishes
	// if it has a duration of 0.0, it still completes one frame
	inline bool IsComplete() const
	{
		if(mDuration < 0.0f)
		{
			return false;
		}
		return mTimeElapsed > mDuration;
	};
	inline float FractionComplete() const
	{
		if(IsComplete())
		{
			return 1.0f;
		}
		return  mTimeElapsed/mDuration;
	}
	virtual void SetDuration(float duration)
	{
		mDuration = duration;
	}
protected:

	float mDuration;
private:
	float mTimeElapsed;
	const char* mName;
};

