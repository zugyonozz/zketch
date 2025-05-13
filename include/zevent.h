// zevent.h
#pragma once
#include "zenv.h"
#include "zbj.h"
#include <functional>
#include <tuple>
#include <utility>
#include <cstdint>
#include <initializer_list>

class zFlow {
private:
    std::function<bool()> condition;
    std::function<void()> funcYes;
    std::function<void()> funcNo;

public:
    // Default constructor
    zFlow() = default;
    
    // Construct from condition and two functions
    template <typename Cond, typename YesFunc, typename NoFunc>
    zFlow(Cond&& cond, YesFunc&& yes, NoFunc&& no = [](){}) :
        condition(std::forward<Cond>(cond)),
        funcYes(std::forward<YesFunc>(yes)),
        funcNo(std::forward<NoFunc>(no))
    {}
    
    // Construct from std::pair structure
    zFlow(const std::pair<std::function<bool()>, 
          std::pair<std::function<void()>, std::function<void()>>>& flow) :
        condition(flow.first),
        funcYes(flow.second.first),
        funcNo(flow.second.second)
    {}
    
    // Helper function to create from condition and two functions
    template <typename Cond, typename YesFunc, typename NoFunc>
    static zFlow create(Cond&& cond, YesFunc&& yes, NoFunc&& no) {
        return zFlow(std::forward<Cond>(cond),
                    std::forward<YesFunc>(yes),
                    std::forward<NoFunc>(no));
    }
    
    // Helper function to create with arguments for yes and no functions
    template <typename Cond, typename YesFunc, typename... YesArgs,
              typename NoFunc, typename... NoArgs>
    static zFlow createWithArgs(
        Cond&& cond,
        YesFunc&& yes, std::tuple<YesArgs...> yesArgs,
        NoFunc&& no, std::tuple<NoArgs...> noArgs
    ) {
        auto boundYes = [yes, yesArgs]() { std::apply(yes, yesArgs); };
        auto boundNo = [no, noArgs]() { std::apply(no, noArgs); };
        
        return zFlow(std::forward<Cond>(cond), boundYes, boundNo);
    }
    
    // Operator to execute the flow
    void operator()() const {
        if (!condition) return;
        
        if (condition()) {
            if (funcYes) funcYes();
        } else {
            if (funcNo) funcNo();
        }
    }
    
    // Execute method as an alternative to operator()
    void execute() const {
        this->operator()();
    }
    
    // Fluent setter methods
    zFlow& setCondition(std::function<bool()> cond) {
        condition = cond;
        return *this;
    }
    
    zFlow& setYesAction(std::function<void()> action) {
        funcYes = action;
        return *this;
    }
    
    zFlow& setNoAction(std::function<void()> action) {
        funcNo = action;
        return *this;
    }
};

// Helper function to create a zFlow from a pair structure
inline zFlow makeFlow(const std::pair<std::function<bool()>, 
                     std::pair<std::function<void()>, std::function<void()>>>& flowPair) {
    return zFlow(flowPair);
}

class zFuncs {
public:
    // Default constructor
    zFuncs() = default;
    
    // Constructor with initializer list of functions
    zFuncs(std::initializer_list<std::function<void()>> funcList) {
        for (const auto& func : funcList) {
            funcs.push_back(func);
        }
    }
    
    // Add a single function
    void add(std::function<void()> func) { 
        funcs.push_back(std::move(func)); 
    }

    // Add a function with arguments (bind the arguments)
    template <typename Func, typename... Args>
    void add(Func&& func, Args&&... args) {
        funcs.push_back([=]() { func(args...); });
    }
    
    // Execute all functions
    void operator()() { 
        for (auto& f : funcs) { 
            f(); 
        } 
    }
    
    // Get number of functions
    size_t size() const {
        return funcs.size();
    }
    
    // Execute a specific function by index
    void execute(size_t index) {
        if (index < funcs.size()) {
            funcs[index]();
        }
    }
    
    // Clear all functions
    void clear() {
        funcs.clear();
    }

private:
    std::vector<std::function<void()>> funcs;
};

FPoint getMousePos(){
	FPoint coord;
	SDL_PumpEvents();
	SDL_GetMouseState(&coord.x, &coord.y);
	return coord;
}

enum clickState : uint32_t {
	NONE		= 0,		// 0000
	CLICKED		= 1 << 0,	// 0001
	INBOUND		= 1 << 1,	// 0010
	OUTBOUND	= 1 << 2	// 0100
};

uint32_t getClickState(const Event& e, Bound b = {}){
	if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
		int mx = e.button.x;
		int my = e.button.y;
		if(mx >= b.x && mx <= b.x + b.w && my >= b.y && my <= b.y + b.h){
			std::cout << "click inside bound!" << std::endl;
			return clickState::CLICKED | clickState::INBOUND;
		}else{
			std::cout << "click outside bound!" << std::endl;
			return clickState::CLICKED | clickState::OUTBOUND;
		}
	}
	return clickState::NONE;
}

bool isHover(Bound b){
	FPoint coord = getMousePos();
	if(coord.x >= b.x && coord.x <= b.x + b.w && coord.y >= b.y && coord.y <= b.y + b.h){
		return true;
	}
	return false;
}

bool isOnClick(const Event& e, Bound b){
	if(getClickState(e, b) == 0){
		return false;
	}
	return true;
}