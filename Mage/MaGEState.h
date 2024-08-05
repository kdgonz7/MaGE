namespace MaGE {
	enum M_State {
		S_SLEEPING = 0,
		S_INSTRUCTION = 1,
		S_TRAP = 2,
		S_RETURNFROMTRAP = 3,
		S_FINISHED = 4
	};

	class MaGEState {
		M_State m_state = S_SLEEPING;
	public:
		MaGEState() = default;
		~MaGEState() = default;

		M_State getState() { return m_state; }
		bool set(M_State state);
	};
}