#include "utility.hpp"
#include <functional>
#include <cstddef>
#include "exception.hpp"
#include <map>
#include <cstdio>
namespace sjtu {
	const char BPTREE_ADDRESS[128] = "bpt";
	template <class Key, class Value, class Compare = std::less<Key> >
	class BTree {
	public:
		typedef pair<const Key, Value> value_type;

		class const_iterator;
		class iterator {
			friend class sjtu::BTree<Key, Value, Compare>::const_iterator;
			friend iterator sjtu::BTree<Key, Value, Compare>::begin();
			friend iterator sjtu::BTree<Key, Value, Compare>::end();
			friend iterator sjtu::BTree<Key, Value, Compare>::find(const Key&);
			friend pair<iterator, OperationResult> sjtu::BTree<Key, Value, Compare>::insert(const Key&, const Value&);
		private:
			BTree* cur_bptree = nullptr;
			kuaihead block_info;
			int cur_pos = 0;

		public:
			bool modify(const Value& value) {
				kuaihead info;
				yeshuju leafData;
				dukuai(&info, &leafData, block_info.headpos);
				leafData.val[cur_pos].second = value;
				xiekuai(&info, &leafData, block_info.headpos);
				return true;
			}
			iterator() {
				// TODO Default Constructor
			}
			iterator(const iterator& other) {
				// TODO Copy Constructor
				cur_bptree = other.cur_bptree;
				block_info = other.block_info;
				cur_pos = other.cur_pos;
			}
			// Return a new iterator which points to the n-next elements
			iterator operator++(int) {
				// Todo iterator++
				auto temp = *this;
				++cur_pos;
				if (cur_pos >= block_info.headsize) {
					char buff[kuaisize] = { 0 };
					memdu(buff, kuaisize, block_info.headnext);
					memcpy(&block_info, buff, sizeof(block_info));
					cur_pos = 0;
				}
				return temp;
			}
			iterator& operator++() {
				// Todo ++iterator
				++cur_pos;
				if (cur_pos >= block_info.headsize) {
					char buff[kuaisize] = { 0 };
					memdu(buff, kuaisize, block_info.headnext);
					memcpy(&block_info, buff, sizeof(block_info));
					cur_pos = 0;
				}
				return *this;
			}
			iterator operator--(int) {
				// Todo iterator--
				auto temp = *this;
				if (cur_pos == 0) {
					char buff[kuaisize] = { 0 };
					memdu(buff, kuaisize, block_info.headpre);
					memcpy(&block_info, buff, sizeof(block_info));
					cur_pos = block_info.headsize - 1;
				}
				else
					--cur_pos;
				return temp;
			}
			iterator& operator--() {
				// Todo --iterator
				if (cur_pos == 0) {
					char buff[kuaisize] = { 0 };
					memdu(buff, kuaisize, block_info.headpre);
					memcpy(&block_info, buff, sizeof(block_info));
					cur_pos = block_info.headsize - 1;
				}
				else
					--cur_pos;

				return *this;
			}
			// Overloaded of operator '==' and '!='
			// Check whether the iterators are same
			value_type operator*() const {
				// Todo operator*, return the <K,V> of iterator
				if (cur_pos >= block_info.headsize)
					throw invalid_iterator();
				char buff[kuaisize] = { 0 };
				memdu(buff, kuaisize, block_info.headpos);
				yeshuju leafData;
				memcpy(&leafData, buff + initsize, sizeof(leafData));
				value_type result(leafData.val[cur_pos].first, leafData.val[cur_pos].second);
				return result;
			}
			bool operator==(const iterator & rhs) const {
				// Todo operator ==
				return cur_bptree == rhs.cur_bptree
					&& block_info.headpos == rhs.block_info.headpos
					&& cur_pos == rhs.cur_pos;
			}
			bool operator==(const const_iterator & rhs) const {
				// Todo operator ==
				return block_info.headpos == rhs.block_info.headpos
					&& cur_pos == rhs.cur_pos;
			}
			bool operator!=(const iterator & rhs) const {
				// Todo operator !=
				return cur_bptree != rhs.cur_bptree || block_info.headpos != rhs.block_info.headpos || cur_pos != rhs.cur_pos;
			}
			bool operator!=(const const_iterator & rhs) const {
				// Todo operator !=
				return block_info.headpos != rhs.block_info.headpos || cur_pos != rhs.cur_pos;
			}
		};
		class const_iterator {
			// it should has similar member method as iterator.
			//  and it should be able to construct from an iterator.
			friend class sjtu::BTree<Key, Value, Compare>::iterator;
			friend const_iterator sjtu::BTree<Key, Value, Compare>::cbegin() const;
			friend const_iterator sjtu::BTree<Key, Value, Compare>::cend() const;
			friend const_iterator sjtu::BTree<Key, Value, Compare>::find(const Key&) const;
		private:
			// Your private members go here
			kuaihead block_info;
			int cur_pos = 0;
		public:
			const_iterator() {
				// TODO
			}
			const_iterator(const const_iterator& other) {
				// TODO
				block_info = other.block_info;
				cur_pos = other.cur_pos;
			}
			const_iterator(const iterator& other) {
				// TODO
				block_info = other.block_info;
				cur_pos = other.cur_pos;
			}
			// And other methods in iterator, please fill by yourself.
			// Return a new iterator which points to the n-next elements
			const_iterator operator++(int) {
				// Todo iterator++
				auto temp = *this;
				++cur_pos;
				if (cur_pos >= block_info.headsize) {
					char buff[kuaisize] = { 0 };
					memdu(buff, kuaisize, block_info.headnext);
					memcpy(&block_info, buff, sizeof(block_info));
					cur_pos = 0;
				}
				return temp;
			}
			const_iterator& operator++() {
				// Todo ++iterator
				++cur_pos;
				if (cur_pos >= block_info.headsize) {
					char buff[kuaisize] = { 0 };
					memdu(buff, kuaisize, block_info.headnext);
					memcpy(&block_info, buff, sizeof(block_info));
					cur_pos = 0;
				}
				return *this;
			}
			const_iterator operator--(int) {
				// Todo iterator--
				auto temp = *this;
				if (cur_pos == 0) {
					char buff[kuaisize] = { 0 };
					memdu(buff, kuaisize, block_info.headpre);
					memcpy(&block_info, buff, sizeof(block_info));
					cur_pos = block_info.headsize - 1;
				}
				else
					--cur_pos;
				return temp;
			}
			const_iterator& operator--() {
				// Todo --iterator
				if (cur_pos == 0) {
					char buff[kuaisize] = { 0 };
					memdu(buff, kuaisize, block_info.headpre);
					memcpy(&block_info, buff, sizeof(block_info));
					cur_pos = block_info.headsize - 1;
				}
				else
					--cur_pos;

				return *this;
			}
			// Overloaded of operator '==' and '!='
			// Check whether the iterators are same
			value_type operator*() const {
				// Todo operator*, return the <K,V> of iterator
				if (cur_pos >= block_info.headsize)
					throw invalid_iterator();
				char buff[kuaisize] = { 0 };
				memdu(buff, kuaisize, block_info.headpos);
				yeshuju leafData;
				memcpy(&leafData, buff + initsize, sizeof(leafData));
				value_type result(leafData.val[cur_pos].first, leafData.val[cur_pos].second);
				return result;
			}
			bool operator==(const iterator & rhs) const {
				// Todo operator ==
				return block_info.headpos == rhs.block_info.headpos
					&& cur_pos == rhs.cur_pos;
			}
			bool operator==(const const_iterator & rhs) const {
				// Todo operator ==
				return block_info.headpos == rhs.block_info.headpos
					&& cur_pos == rhs.cur_pos;
			}
			bool operator!=(const iterator & rhs) const {
				// Todo operator !=
				return block_info.headpos != rhs.block_info.headpos
					|| cur_pos != rhs.cur_pos;
			}
			bool operator!=(const const_iterator & rhs) const {
				// Todo operator !=
				return block_info.headpos != rhs.block_info.headpos
					|| cur_pos != rhs.cur_pos;
			}
		};
	private:
		class kuaihead {
		public:
			bool headbz = false;
			int headsize = 0;
			int headpos = 0;
			int headpar = 0;
			int headpre = 0;
			int headnext = 0;
		};

		struct shujunode {
			int nodeerzi = 0;
			Key nodekey;
		};
		const static int kuaisize = 4096;
		const static int initsize = sizeof(kuaihead);
		const static int keysize = sizeof(Key);
		const static int valuesize = sizeof(Value);
		const static int kuaikeynum = (kuaisize - initsize) / sizeof(shujunode) - 1;
		const static int kuaipnum = (kuaisize - initsize) / (keysize + valuesize) - 1;
		class filehead {
		public:
			int block_cnt = 1;
			int root_pos = 0;
			int data_blockHead = 0;
			int data_block_rear = 0;
			int headsize = 0;
		};

		class shujukuai {
		public:
			shujunode val[kuaikeynum];
		};

		class yeshuju {
		public:
			pair<Key, Value> val[kuaipnum];
		};

		filehead bptinfo;

		static FILE* wenjian;

		template <class MEM_TYPE>
		static void memdu(MEM_TYPE buff, int buff_size, int pos) {
			fseek(wenjian, long(buff_size * pos), SEEK_SET);
			fread(buff, buff_size, 1, wenjian);
		}

		template <class MEM_TYPE>
		static void memxie(MEM_TYPE buff, int buff_size, int pos) {
			fseek(wenjian, long(buff_size * pos), SEEK_SET);
			fwrite(buff, buff_size, 1, wenjian);
			fflush(wenjian);
		}

		void xiexinxi() {
			fseek(wenjian, 0, SEEK_SET);
			char buff[kuaisize] = { 0 };
			memcpy(buff, &bptinfo, sizeof(bptinfo));
			memxie(buff, kuaisize, 0);
		}

		int getm() {
			++bptinfo.block_cnt;
			xiexinxi();
			char buff[kuaisize] = { 0 };
			memxie(buff, kuaisize, bptinfo.block_cnt - 1);
			return bptinfo.block_cnt - 1;
		}

		int jiannode(int headpar) {
			auto node_pos = getm();
			kuaihead temp;
			shujukuai normalData;
			temp.headbz = false;
			temp.headpar = headpar;
			temp.headpos = node_pos;
			temp.headsize = 0;
			xiekuai(&temp, &normalData, node_pos);
			return node_pos;
		}

		int jianyezi(int headpar, int headpre, int headnext) {
			auto node_pos = getm();
			kuaihead temp;
			yeshuju leafData;
			temp.headbz = true;
			temp.headpar = headpar;
			temp.headpos = node_pos;
			temp.headpre = headpre;
			temp.headnext = headnext;
			temp.headsize = 0;
			xiekuai(&temp, &leafData, node_pos);
			return node_pos;
		}

		void charuxinbz(kuaihead & parent_info, shujukuai & parent_data,
			int origin, int new_pos, const Key & new_index) {
			++parent_info.headsize;
			auto p = parent_info.headsize - 2;
			for (; parent_data.val[p].nodeerzi != origin; --p) {
				parent_data.val[p + 1] = parent_data.val[p];
			}
			parent_data.val[p + 1].nodekey = parent_data.val[p].nodekey;
			parent_data.val[p].nodekey = new_index;
			parent_data.val[p + 1].nodeerzi = new_pos;
		}

		template <class DATA_TYPE>
		static void xiekuai(kuaihead * _info, DATA_TYPE * _data, int headpos) {
			char buff[kuaisize] = { 0 };
			memcpy(buff, _info, sizeof(kuaihead));
			memcpy(buff + initsize, _data, sizeof(DATA_TYPE));
			memxie(buff, kuaisize, headpos);
		}

		template <class DATA_TYPE>
		static void dukuai(kuaihead * _info, DATA_TYPE * _data, int headpos) {
			char buff[kuaisize] = { 0 };
			memdu(buff, kuaisize, headpos);
			memcpy(_info, buff, sizeof(kuaihead));
			memcpy(_data, buff + initsize, sizeof(DATA_TYPE));
		}

		Key split_leaf_node(int pos, kuaihead & origin_info, yeshuju & origin_data) {
			int parent_pos;
			kuaihead parent_info;
			shujukuai parent_data;

			if (pos == bptinfo.root_pos) {
				auto root_pos = jiannode(0);
				bptinfo.root_pos = root_pos;
				xiexinxi();
				dukuai(&parent_info, &parent_data, root_pos);
				origin_info.headpar = root_pos;
				++parent_info.headsize;
				parent_data.val[0].nodeerzi = pos;
				parent_pos = root_pos;
			}
			else {
				dukuai(&parent_info, &parent_data, origin_info.headpar);
				parent_pos = parent_info.headpos;
			}
			if (jiancepar(origin_info)) {
				parent_pos = origin_info.headpar;
				dukuai(&parent_info, &parent_data, parent_pos);
			}
			auto new_pos = jianyezi(parent_pos, pos, origin_info.headnext);

			auto temp_pos = origin_info.headnext;
			kuaihead temp_info;
			yeshuju temp_data;
			dukuai(&temp_info, &temp_data, temp_pos);
			temp_info.headpre = new_pos;
			xiekuai(&temp_info, &temp_data, temp_pos);
			origin_info.headnext = new_pos;

			kuaihead new_info;
			yeshuju new_data;
			dukuai(&new_info, &new_data, new_pos);

			int mid_pos = origin_info.headsize >> 1;
			for (int p = mid_pos, i = 0; p < origin_info.headsize; ++p, ++i) {
				new_data.val[i].first = origin_data.val[p].first;
				new_data.val[i].second = origin_data.val[p].second;
				++new_info.headsize;
			}
			origin_info.headsize = mid_pos;
			charuxinbz(parent_info, parent_data, pos, new_pos, origin_data.val[mid_pos].first);

			xiekuai(&origin_info, &origin_data, pos);
			xiekuai(&new_info, &new_data, new_pos);
			xiekuai(&parent_info, &parent_data, parent_pos);

			return new_data.val[0].first;
		}

		bool jiancepar(kuaihead & child_info) {
			int parent_pos, origin_pos = child_info.headpar;
			kuaihead parent_info, origin_info;
			shujukuai parent_data, origin_data;
			dukuai(&origin_info, &origin_data, origin_pos);
			if (origin_info.headsize < kuaikeynum)
				return false;

			if (origin_pos == bptinfo.root_pos) {
				auto root_pos = jiannode(0);
				bptinfo.root_pos = root_pos;
				xiexinxi();
				dukuai(&parent_info, &parent_data, root_pos);
				origin_info.headpar = root_pos;
				++parent_info.headsize;
				parent_data.val[0].nodeerzi = origin_pos;
				parent_pos = root_pos;
			}
			else {
				dukuai(&parent_info, &parent_data, origin_info.headpar);
				parent_pos = parent_info.headpos;
			}
			if (jiancepar(origin_info)) {
				parent_pos = origin_info.headpar;
				dukuai(&parent_info, &parent_data, parent_pos);
			}
			auto new_pos = jiannode(parent_pos);
			kuaihead new_info;
			shujukuai new_data;
			dukuai(&new_info, &new_data, new_pos);

			int mid_pos = origin_info.headsize >> 1;
			for (int p = mid_pos + 1, i = 0; p < origin_info.headsize; ++p, ++i) {
				if (origin_data.val[p].nodeerzi == child_info.headpos) {
					child_info.headpar = new_pos;
				}
				std::swap(new_data.val[i], origin_data.val[p]);
				++new_info.headsize;
			}
			origin_info.headsize = mid_pos + 1;
			charuxinbz(parent_info, parent_data, origin_pos, new_pos, origin_data.val[mid_pos].nodekey);

			xiekuai(&origin_info, &origin_data, origin_pos);
			xiekuai(&new_info, &new_data, new_pos);
			xiekuai(&parent_info, &parent_data, parent_pos);
			return true;
		}

		void bianbz(int l_parent, int l_child, const Key & new_key) {
			kuaihead parent_info;
			shujukuai parent_data;
			dukuai(&parent_info, &parent_data, l_parent);
			if (parent_data.val[parent_info.headsize - 1].nodeerzi == l_child) {
				bianbz(parent_info.headpar, l_parent, new_key);
				return;
			}
			for (int cur_pos = parent_info.headsize - 2;; --cur_pos) {
				if (parent_data.val[cur_pos].nodeerzi == l_child) {
					parent_data.val[cur_pos].nodekey = new_key;
					break;
				}
			}
			xiekuai(&parent_info, &parent_data, l_parent);
		}

		void hebing(kuaihead & l_info, shujukuai & l_data, kuaihead & r_info, shujukuai & r_data) {
			for (int p = l_info.headsize, i = 0; i < r_info.headsize; ++p, ++i) {
				l_data.val[p] = r_data.val[i];
			}
			l_data.val[l_info.headsize - 1].nodekey = adjust(r_info.headpar, r_info.headpos);
			l_info.headsize += r_info.headsize;
			xiekuai(&l_info, &l_data, l_info.headpos);
		}

		void pingheng(kuaihead & info, shujukuai & normalData) {
			if (info.headsize >= kuaikeynum / 2) {
				xiekuai(&info, &normalData, info.headpos);
				return;
			}
			if (info.headpos == bptinfo.root_pos && info.headsize <= 1) {
				bptinfo.root_pos = normalData.val[0].nodeerzi;
				xiexinxi();
				return;
			}
			else if (info.headpos == bptinfo.root_pos) {
				xiekuai(&info, &normalData, info.headpos);
				return;
			}

			kuaihead parent_info, brother_info;
			shujukuai parent_data, brother_data;
			dukuai(&parent_info, &parent_data, info.headpar);
			int value_pos;
			for (value_pos = 0; parent_data.val[value_pos].nodeerzi != info.headpos; ++value_pos);
			if (value_pos > 0) {
				dukuai(&brother_info, &brother_data, parent_data.val[value_pos - 1].nodeerzi);
				brother_info.headpar = info.headpar;
				if (brother_info.headsize > kuaikeynum / 2) {
					for (int p = info.headsize; p > 0; --p) {
						normalData.val[p] = normalData.val[p - 1];
					}
					normalData.val[0].nodeerzi = brother_data.val[brother_info.headsize - 1].nodeerzi;
					normalData.val[0].nodekey = parent_data.val[value_pos - 1].nodekey;
					parent_data.val[value_pos - 1].nodekey = brother_data.val[brother_info.headsize - 2].nodekey;
					--brother_info.headsize;
					++info.headsize;
					xiekuai(&brother_info, &brother_data, brother_info.headpos);
					xiekuai(&info, &normalData, info.headpos);
					xiekuai(&parent_info, &parent_data, parent_info.headpos);
					return;
				}
				else {
					hebing(brother_info, brother_data, info, normalData);
					return;
				}
			}
			if (value_pos < parent_info.headsize - 1) {
				dukuai(&brother_info, &brother_data, parent_data.val[value_pos + 1].nodeerzi);
				brother_info.headpar = info.headpar;
				if (brother_info.headsize > kuaikeynum / 2) {
					normalData.val[info.headsize].nodeerzi = brother_data.val[0].nodeerzi;
					normalData.val[info.headsize - 1].nodekey = parent_data.val[value_pos].nodekey;
					parent_data.val[value_pos].nodekey = brother_data.val[0].nodekey;
					for (int p = 1; p < brother_info.headsize; ++p) {
						brother_data.val[p - 1] = brother_data.val[p];
					}
					--brother_info.headsize;
					++info.headsize;
					xiekuai(&brother_info, &brother_data, brother_info.headpos);
					xiekuai(&info, &normalData, info.headpos);
					xiekuai(&parent_info, &parent_data, parent_info.headpos);
					return;
				}
				else {
					hebing(info, normalData, brother_info, brother_data);
					return;
				}
			}
		}

		Key adjust(int pos, int removed_child) {
			kuaihead info;
			shujukuai normalData;
			dukuai(&info, &normalData, pos);
			int cur_pos;
			for (cur_pos = 0; normalData.val[cur_pos].nodeerzi != removed_child; ++cur_pos);
			Key ans = normalData.val[cur_pos - 1].nodekey;
			normalData.val[cur_pos - 1].nodekey = normalData.val[cur_pos].nodekey;
			for (; cur_pos < info.headsize - 1; ++cur_pos) {
				normalData.val[cur_pos] = normalData.val[cur_pos + 1];
			}
			--info.headsize;
			pingheng(info, normalData);
			return ans;
		}

		void hebingyezi(kuaihead & l_info, yeshuju & l_data, kuaihead & r_info, yeshuju & r_data) {
			for (int p = l_info.headsize, i = 0; i < r_info.headsize; ++p, ++i) {
				l_data.val[p].first = r_data.val[i].first;
				l_data.val[p].second = r_data.val[i].second;
			}
			l_info.headsize += r_info.headsize;
			adjust(r_info.headpar, r_info.headpos);
			l_info.headnext = r_info.headnext;
			kuaihead temp_info;
			yeshuju temp_data;
			dukuai(&temp_info, &temp_data, r_info.headnext);
			temp_info.headpre = l_info.headpos;
			xiekuai(&temp_info, &temp_data, temp_info.headpos);
			xiekuai(&l_info, &l_data, l_info.headpos);
		}

		void pinghengyezi(kuaihead & leaf_info, yeshuju & leafData) {
			if (leaf_info.headsize >= kuaipnum / 2) {
				xiekuai(&leaf_info, &leafData, leaf_info.headpos);
				return;
			}
			else if (leaf_info.headpos == bptinfo.root_pos) {
				if (leaf_info.headsize == 0) {
					kuaihead temp_info;
					yeshuju temp_data;
					dukuai(&temp_info, &temp_data, bptinfo.data_blockHead);
					temp_info.headnext = bptinfo.data_block_rear;
					xiekuai(&temp_info, &temp_data, bptinfo.data_blockHead);
					dukuai(&temp_info, &temp_data, bptinfo.data_block_rear);
					temp_info.headpre = bptinfo.data_blockHead;
					xiekuai(&temp_info, &temp_data, bptinfo.data_block_rear);
					return;
				}
				xiekuai(&leaf_info, &leafData, leaf_info.headpos);
				return;
			}

			kuaihead brother_info, parent_info;
			yeshuju brother_data;
			shujukuai parent_data;

			dukuai(&parent_info, &parent_data, leaf_info.headpar);
			int node_pos = 0;
			for (; node_pos < parent_info.headsize; ++node_pos) {
				if (parent_data.val[node_pos].nodeerzi == leaf_info.headpos)
					break;
			}

			if (node_pos > 0) {
				dukuai(&brother_info, &brother_data, leaf_info.headpre);
				brother_info.headpar = leaf_info.headpar;
				if (brother_info.headsize > kuaipnum / 2) {
					for (int p = leaf_info.headsize; p > 0; --p) {
						leafData.val[p].first = leafData.val[p - 1].first;
						leafData.val[p].second = leafData.val[p - 1].second;
					}
					leafData.val[0].first = brother_data.val[brother_info.headsize - 1].first;
					leafData.val[0].second = brother_data.val[brother_info.headsize - 1].second;
					--brother_info.headsize;
					++leaf_info.headsize;
					bianbz(brother_info.headpar, brother_info.headpos, leafData.val[0].first);
					xiekuai(&brother_info, &brother_data, brother_info.headpos);
					xiekuai(&leaf_info, &leafData, leaf_info.headpos);
					return;
				}
				else {
					hebingyezi(brother_info, brother_data, leaf_info, leafData);
					return;
				}
			}

			if (node_pos < parent_info.headsize - 1) {
				dukuai(&brother_info, &brother_data, leaf_info.headnext);
				brother_info.headpar = leaf_info.headpar;
				if (brother_info.headsize > kuaipnum / 2) {
					leafData.val[leaf_info.headsize].first = brother_data.val[0].first;
					leafData.val[leaf_info.headsize].second = brother_data.val[0].second;
					for (int p = 1; p < brother_info.headsize; ++p) {
						brother_data.val[p - 1].first = brother_data.val[p].first;
						brother_data.val[p - 1].second = brother_data.val[p].second;
					}
					++leaf_info.headsize;
					--brother_info.headsize;
					bianbz(leaf_info.headpar, leaf_info.headpos, brother_data.val[0].first);
					xiekuai(&leaf_info, &leafData, leaf_info.headpos);
					xiekuai(&brother_info, &brother_data, brother_info.headpos);
					return;
				}
				else {
					hebingyezi(leaf_info, leafData, brother_info, brother_data);
					return;
				}
			}
		}

		void chawenjian() {
			if (!wenjian) {
				wenjian = fopen(BPTREE_ADDRESS, "wb+");
				xiexinxi();

				auto node_head = bptinfo.block_cnt,
					node_rear = bptinfo.block_cnt + 1;

				bptinfo.data_blockHead = node_head;
				bptinfo.data_block_rear = node_rear;

				jianyezi(0, 0, node_rear);
				jianyezi(0, node_head, 0);

				return;
			}
			char buff[kuaisize] = { 0 };
			memdu(buff, kuaisize, 0);
			memcpy(&bptinfo, buff, sizeof(bptinfo));
		}
		BTree() {
			// Todo Default
			wenjian = fopen(BPTREE_ADDRESS, "rb+");
			if (!wenjian) {
				wenjian = fopen(BPTREE_ADDRESS, "wb+");
				xiexinxi();

				auto node_head = bptinfo.block_cnt,
					node_rear = bptinfo.block_cnt + 1;

				bptinfo.data_blockHead = node_head;
				bptinfo.data_block_rear = node_rear;

				jianyezi(0, 0, node_rear);
				jianyezi(0, node_head, 0);

				return;
			}
			char buff[kuaisize] = { 0 };
			memdu(buff, kuaisize, 0);
			memcpy(&bptinfo, buff, sizeof(bptinfo));
		}
		BTree(const BTree& other) {
			wenjian = fopen(BPTREE_ADDRESS, "rb+");
			bptinfo.block_cnt = other.bptinfo.block_cnt;
			bptinfo.data_blockHead = other.bptinfo.data_blockHead;
			bptinfo.data_block_rear = other.bptinfo.data_block_rear;
			bptinfo.root_pos = other.bptinfo.root_pos;
			bptinfo.headsize = other.bptinfo.headsize;
		}
		BTree& operator=(const BTree& other) {
			wenjian = fopen(BPTREE_ADDRESS, "rb+");
			bptinfo.block_cnt = other.bptinfo.block_cnt;
			bptinfo.data_blockHead = other.bptinfo.data_blockHead;
			bptinfo.data_block_rear = other.bptinfo.data_block_rear;
			bptinfo.root_pos = other.bptinfo.root_pos;
			bptinfo.headsize = other.bptinfo.headsize;
			return *this;
		}
		~BTree() {
			fclose(wenjian);
		}
		pair<iterator, OperationResult> insert(const Key& key, const Value& value) {
			// TODO insert function
			chawenjian();
			if (empty()) {
				auto root_pos = jianyezi(0, bptinfo.data_blockHead, bptinfo.data_block_rear);

				kuaihead temp_info;
				yeshuju temp_data;
				dukuai(&temp_info, &temp_data, bptinfo.data_blockHead);
				temp_info.headnext = root_pos;
				xiekuai(&temp_info, &temp_data, bptinfo.data_blockHead);

				dukuai(&temp_info, &temp_data, bptinfo.data_block_rear);
				temp_info.headpre = root_pos;
				xiekuai(&temp_info, &temp_data, bptinfo.data_block_rear);

				dukuai(&temp_info, &temp_data, root_pos);
				++temp_info.headsize;
				temp_data.val[0].first = key;
				temp_data.val[0].second = value;
				xiekuai(&temp_info, &temp_data, root_pos);

				++bptinfo.headsize;
				bptinfo.root_pos = root_pos;
				xiexinxi();

				pair<iterator, OperationResult> result(begin(), Success);
				return result;
			}

			char buff[kuaisize] = { 0 };
			int cur_pos = bptinfo.root_pos, cur_parent = 0;
			while (true) {
				memdu(buff, kuaisize, cur_pos);
				kuaihead temp;
				memcpy(&temp, buff, sizeof(temp));
				if (cur_parent != temp.headpar) {
					temp.headpar = cur_parent;
					memcpy(buff, &temp, sizeof(temp));
					memxie(buff, kuaisize, cur_pos);
				}
				if (temp.headbz) {
					break;
				}
				shujukuai normalData;
				memcpy(&normalData, buff + initsize, sizeof(normalData));
				int child_pos = temp.headsize - 1;
				for (; child_pos > 0; --child_pos) {
					if (!(normalData.val[child_pos - 1].nodekey > key)) {
						break;
					}
				}
				cur_parent = cur_pos;
				cur_pos = normalData.val[child_pos].nodeerzi;
			}

			kuaihead info;
			memcpy(&info, buff, sizeof(info));
			yeshuju leafData;
			memcpy(&leafData, buff + initsize, sizeof(leafData));
			for (int value_pos = 0;; ++value_pos) {
				if (value_pos < info.headsize && (!(leafData.val[value_pos].first<key || leafData.val[value_pos].first>key))) {
					//throw runtime_error();
					return pair<iterator, OperationResult>(end(), Fail);
				}
				if (value_pos >= info.headsize || leafData.val[value_pos].first > key) {
					if (info.headsize >= kuaipnum) {
						auto cur_key = split_leaf_node(cur_pos, info, leafData);
						if (key > cur_key) {
							cur_pos = info.headnext;
							value_pos -= info.headsize;
							dukuai(&info, &leafData, cur_pos);
						}
					}

					for (int p = info.headsize - 1; p >= value_pos; --p) {
						leafData.val[p + 1].first = leafData.val[p].first;
						leafData.val[p + 1].second = leafData.val[p].second;
						if (p == value_pos)
							break;
					}
					leafData.val[value_pos].first = key;
					leafData.val[value_pos].second = value;
					++info.headsize;
					xiekuai(&info, &leafData, cur_pos);
					iterator ans;
					ans.block_info = info;
					ans.cur_bptree = this;
					ans.cur_pos = value_pos;
					++bptinfo.headsize;
					xiexinxi();
					pair<iterator, OperationResult> to_return(ans, Success);
					return to_return;
				}
			}
			return pair<iterator, OperationResult>(end(), Fail);
		}
		OperationResult erase(const Key & key) {
			chawenjian();
			// TODO erase function
			if (empty()) {
				return Fail;
			}
			char buff[kuaisize] = { 0 };
			int cur_pos = bptinfo.root_pos, cur_parent = 0;
			while (true) {
				memdu(buff, kuaisize, cur_pos);
				kuaihead temp;
				memcpy(&temp, buff, sizeof(temp));
				if (cur_parent != temp.headpar) {
					temp.headpar = cur_parent;
					memcpy(buff, &temp, sizeof(temp));
					memxie(buff, kuaisize, cur_pos);
				}
				if (temp.headbz) {
					break;
				}
				shujukuai normalData;
				memcpy(&normalData, buff + initsize, sizeof(normalData));
				int child_pos = temp.headsize - 1;
				for (; child_pos > 0; --child_pos) {
					if (!(normalData.val[child_pos - 1].nodekey > key)) {
						break;
					}
				}
				cur_parent = cur_pos;
				cur_pos = normalData.val[child_pos].nodeerzi;
			}

			kuaihead info;
			memcpy(&info, buff, sizeof(info));
			yeshuju leafData;
			memcpy(&leafData, buff + initsize, sizeof(leafData));
			for (int value_pos = 0;; ++value_pos) {
				if (value_pos < info.headsize && (!(leafData.val[value_pos].first<key || leafData.val[value_pos].first>key))) {
					--info.headsize;
					for (int p = value_pos; p < info.headsize; ++p) {
						leafData.val[p].first = leafData.val[p + 1].first;
						leafData.val[p].second = leafData.val[p + 1].second;
					}
					pinghengyezi(info, leafData);
					--bptinfo.headsize;
					xiexinxi();
					return Success;
				}
				if (value_pos >= info.headsize || leafData.val[value_pos].first > key) {
					return Fail;
				}
			}
			return Fail;  
		}
		iterator begin() {
			chawenjian();
			iterator result;
			char buff[kuaisize] = { 0 };
			memdu(buff, kuaisize, bptinfo.data_blockHead);
			kuaihead blockHead;
			memcpy(&blockHead, buff, sizeof(blockHead));
			result.block_info = blockHead;
			result.cur_bptree = this;
			result.cur_pos = 0;
			++result;
			return result;
		}
		const_iterator cbegin() const {
			const_iterator result;
			char buff[kuaisize] = { 0 };
			memdu(buff, kuaisize, bptinfo.data_blockHead);
			kuaihead blockHead;
			memcpy(&blockHead, buff, sizeof(blockHead));
			result.block_info = blockHead;
			result.cur_pos = 0;
			++result;
			return result;
		}
		// Return a iterator to the end(the next element after the prev)
		iterator end() {
			chawenjian();
			iterator result;
			char buff[kuaisize] = { 0 };
			memdu(buff, kuaisize, bptinfo.data_block_rear);
			kuaihead blockHead;
			memcpy(&blockHead, buff, sizeof(blockHead));
			result.block_info = blockHead;
			result.cur_bptree = this;
			result.cur_pos = 0;
			return result;
		}
		const_iterator cend() const {
			const_iterator result;
			char buff[kuaisize] = { 0 };
			memdu(buff, kuaisize, bptinfo.data_block_rear);
			kuaihead blockHead;
			memcpy(&blockHead, buff, sizeof(blockHead));
			result.block_info = blockHead;
			result.cur_pos = 0;
			return result;
		}
		// Check whether this BTree is empty
		bool empty() const {
			if (!wenjian)
				return true;
			return bptinfo.headsize == 0;
		}
		// Return the number of <K,V> pairs
		int size() const {
			if (!wenjian)
				return 0;
			return bptinfo.headsize;
		}
		// Clear the BTree
		void clear() {
			if (!wenjian)
				return;
			remove(BPTREE_ADDRESS);
			filehead new_fileHead;
			bptinfo = new_fileHead;
			wenjian = nullptr;
		}
		// Return the value refer to the Key(key)
		Value at(const Key & key) {
			if (empty()) {
				throw container_is_empty();
			}

			char buff[kuaisize] = { 0 };
			int cur_pos = bptinfo.root_pos, cur_parent = 0;
			while (true) {
				memdu(buff, kuaisize, cur_pos);
				kuaihead temp;
				memcpy(&temp, buff, sizeof(temp));

				if (cur_parent != temp.headpar) {
					temp.headpar = cur_parent;
					memcpy(buff, &temp, sizeof(temp));
					memxie(buff, kuaisize, cur_pos);
				}
				if (temp.headbz) {
					break;
				}
				shujukuai normalData;
				memcpy(&normalData, buff + initsize, sizeof(normalData));
				int child_pos = temp.headsize - 1;
				for (; child_pos > 0; --child_pos) {
					if (!(normalData.val[child_pos - 1].nodekey > key)) {
						break;
					}
				}
				cur_pos = normalData.val[child_pos].nodeerzi;
			}
			kuaihead info;
			memcpy(&info, buff, sizeof(info));
			yeshuju leafData;
			memcpy(&leafData, buff + initsize, sizeof(leafData));
			for (int value_pos = 0;; ++value_pos) {
				if (value_pos < info.headsize && (!(leafData.val[value_pos].first<key || leafData.val[value_pos].first>key))) {
					return leafData.val[value_pos].second;
				}
				if (value_pos >= info.headsize || leafData.val[value_pos].first > key) {
					throw index_out_of_bound();
				}
			}
		}
		/**
		 * Returns the number of elements with key
		 *   that compares equivalent to the specified argument,
		 * The default method of check the equivalence is !(a < b || b > a)
		 */
		int count(const Key & key) const {
			return find(key) == cend() ? 0 : 1;
		}
		/**
		 * Finds an element with key equivalent to key.
		 * key value of the element to search for.
		 * Iterator to an element with key equivalent to key.
		 *   If no such element is found, past-the-end (see end()) iterator is
		 * returned.
		 */
		iterator find(const Key & key) {
			if (empty()) {
				return end();
			}

			char buff[kuaisize] = { 0 };
			int cur_pos = bptinfo.root_pos, cur_parent = 0;
			while (true) {
				memdu(buff, kuaisize, cur_pos);
				kuaihead temp;
				memcpy(&temp, buff, sizeof(temp));

				if (cur_parent != temp.headpar) {
					temp.headpar = cur_parent;
					memcpy(buff, &temp, sizeof(temp));
					memxie(buff, kuaisize, cur_pos);
				}
				if (temp.headbz) {
					break;
				}
				shujukuai normalData;
				memcpy(&normalData, buff + initsize, sizeof(normalData));
				int child_pos = temp.headsize - 1;
				for (; child_pos > 0; --child_pos) {
					if (!(normalData.val[child_pos - 1].nodekey > key)) {
						break;
					}
				}
				cur_pos = normalData.val[child_pos].nodeerzi;
			}
			kuaihead info;
			memcpy(&info, buff, sizeof(info));
			sizeof(shujukuai);
			yeshuju leafData;
			memcpy(&leafData, buff + initsize, sizeof(leafData));
			for (int value_pos = 0;; ++value_pos) {
				if (value_pos < info.headsize && (!(leafData.val[value_pos].first<key || leafData.val[value_pos].first>key))) {
					iterator result;
					result.cur_bptree = this;
					result.block_info = info;
					result.cur_pos = value_pos;
					return result;
				}
				if (value_pos >= info.headsize || leafData.val[value_pos].first > key) {
					return end();
				}
			}
			return end();
		}
		const_iterator find(const Key & key) const {
			if (empty()) {
				return cend();
			}

			char buff[kuaisize] = { 0 };
			int cur_pos = bptinfo.root_pos, cur_parent = 0;
			while (true) {
				memdu(buff, kuaisize, cur_pos);
				kuaihead temp;
				memcpy(&temp, buff, sizeof(temp));

				if (cur_parent != temp.headpar) {
					temp.headpar = cur_parent;
					memcpy(buff, &temp, sizeof(temp));
					memxie(buff, kuaisize, cur_pos);
				}
				if (temp.headbz) {
					break;
				}
				shujukuai normalData;
				memcpy(&normalData, buff + initsize, sizeof(normalData));
				int child_pos = temp.headsize - 1;
				for (; child_pos > 0; --child_pos) {
					if (!(normalData.val[child_pos - 1].nodekey > key)) {
						break;
					}
				}
				cur_pos = normalData.val[child_pos].nodeerzi;
			}
			kuaihead info;
			memcpy(&info, buff, sizeof(info));
			yeshuju leafData;
			memcpy(&leafData, buff + initsize, sizeof(leafData));
			for (int value_pos = 0;; ++value_pos) {
				if (value_pos < info.headsize && (!(leafData.val[value_pos].first<key || leafData.val[value_pos].first>key))) {
					const_iterator result;
					result.block_info = info;
					result.cur_pos = value_pos;
					return result;
				}
				if (value_pos >= info.headsize || leafData.val[value_pos].first > key) {
					return cend();
				}
			}
			return cend();
		}
	};
	template <typename Key, typename Value, typename Compare> FILE* BTree<Key, Value, Compare>::wenjian = nullptr;
}  // namespace sjtu
