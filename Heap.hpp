#include <vector>

template<typename T, typename Comp>
class Heap {
public:
	Heap() = default;
	~Heap() = default;

	inline bool empty() const {
		return m_elements.empty();
	}

	inline T pop() {
		T e = m_elements.front();

		if (size() > 0) {
			swap(0, size());
		}

		m_elements.pop_back();

		if (!empty()) {
			heapify(0);
		}

		return e;
	}

	inline void push(T e) {
		m_elements.push_back(e);
		e->index = size();
		e->index = reorder(e);
	}

	inline unsigned int reorder(T e) {
		unsigned int i;

		for (i = e->index; m_less(e, m_elements[i / 2]); i = i / 2) {
			swap(i / 2, i);

			if (i == 0) {
				break;
			}
		}

		return i;
	}

	void heapify(unsigned int i) {
		unsigned int left = (i == 0 ? 1 : 2 * i);
		unsigned int right = (i == 0 ? 2 : 2 * i + 1);

		if (right < size()) {
			int min = (m_less(m_elements[left], m_elements[right]) ? left : right);

			if (m_less(m_elements[min], m_elements[i])) {
				swap(i, min);
				heapify(min);
			}
		}
		else if (left == size() && m_less(m_elements[left], m_elements[i])) {
			swap(i, left);
		}
	}

private:
	inline size_t size() const {
		return m_elements.size() - 1;
	}

	inline void swap(unsigned int i, unsigned int j) {
		T e = m_elements[i];
		m_elements[i] = m_elements[j];
		m_elements[j] = e;

		m_elements[i]->index = i;
		m_elements[j]->index = j;
	}

	Comp m_less;
	std::vector<T> m_elements;
};