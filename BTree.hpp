#include "utility.hpp"
#include <functional>
#include <cstddef>
#include "exception.hpp"
#include <cstdio>
namespace sjtu {
	template <class Key, class Value>
	constexpr size_t kuaidaxiao() {
		size_t cur_size = 2048;
		for (; cur_size < (sizeof(Key) + sizeof(Value)) * 8; cur_size *= 2);
		return cur_size;
	}
	constexpr char BPTREE_ADDRESS[50] = "bpt";
	template <class Key, class Value, class Compare = std::less<Key> >
	class BTree {
	private:
		class xinxi {
		public:
			bool bz = false;
			size_t geshu = 0;
			size_t weizhi = 0;
			size_t fu = 0;
			size_t wei = 0;
			size_t next = 0;
		};
		struct shujunode {
			size_t erzi = 0;
			Key dkey;
		};
		constexpr static size_t dakuaisize = kuaidaxiao<Key, Value>();
		constexpr static size_t xinxisize = sizeof(xinxi);
		constexpr static size_t keysize = sizeof(Key);
		constexpr static size_t valuesize = sizeof(Value);
		constexpr static size_t keygeshu = (dakuaisize - xinxisize) / sizeof(shujunode) - 1;
		constexpr static size_t pairgeshu = (dakuaisize - xinxisize) / (keysize + valuesize) - 1;
		class ffile {
		public:
			size_t kuaicnt = 1;
			size_t rootpos = 0;
			size_t kuaitou = 0;
			size_t kuaiwei = 0;
			size_t geshu = 0;
		};
		class shuju {
		public:
			shujunode a[keygeshu];
		};

		class yezishuju {
		public:
			pair<Key, Value> a[pairgeshu];
		};
		ffile bptr;
		static FILE* wenjian;
		template <class MEM_TYPE>
		static void memdu(MEM_TYPE fz, size_t fzsize, size_t fzpos) {
			fseek(wenjian, long(fzsize * fzpos), SEEK_SET);
			fread(fz, fzsize, 1, wenjian);
		}
		template <class MEM_TYPE>
		static void memxie(MEM_TYPE fz, size_t fzsize, size_t fzpos) {
			fseek(wenjian, long(fzsize * fzpos), SEEK_SET);
			fwrite(fz, fzsize, 1, wenjian);
			fflush(wenjian);
		}
		void xiexinxi() {
			fseek(wenjian, 0, SEEK_SET);
			char fz[dakuaisize] = { 0 };
			memcpy(fz, &bptr, sizeof(bptr));
			memxie(fz, dakuaisize, 0);
		}
		size_t memfenpei() {
			++bptr.kuaicnt;
			char fz[dakuaisize] = { 0 };
			memxie(fz, dakuaisize, bptr.kuaicnt - 1);
			return bptr.kuaicnt - 1;
		}
		size_t jianmid(size_t fu) {
			auto midpos = memfenpei();
			xinxi temp;
			shuju sj;
			temp.bz = false;
			temp.fu = fu;
			temp.weizhi = midpos;
			temp.geshu = 0;
			xienode(&temp, &sj, midpos);
			return midpos;
		}
		size_t jianyenode(size_t fu, size_t wei, size_t next) {
			auto midpos = memfenpei();
			xinxi temp;
			yezishuju yesj;
			temp.bz = true;
			temp.fu = fu;
			temp.weizhi = midpos;
			temp.wei = wei;
			temp.next = next;
			temp.geshu = 0;
			xienode(&temp, &yesj, midpos);
			return midpos;
		}
		void charubz(xinxi & infor, shuju & parsj, size_t zuzong, size_t xinpos, const Key & xinbz) {
			++infor.geshu;
			auto p = infor.geshu - 2;
			for (; parsj.a[p].erzi != zuzong; --p) {
				parsj.a[p + 1] = parsj.a[p];
			}
			parsj.a[p + 1].dkey = parsj.a[p].dkey;
			parsj.a[p].dkey = xinbz;
			parsj.a[p + 1].erzi = xinpos;
		}
		template <class shujuzhonglei>
		static void xienode(xinxi * infor, shujuzhonglei * nodeshu, size_t weizhi) {
			char fz[dakuaisize] = { 0 };
			memcpy(fz, infor, sizeof(xinxi));
			memcpy(fz + xinxisize, nodeshu, sizeof(shujuzhonglei));
			memxie(fz, dakuaisize, weizhi);
		}
		template <class shujuzhonglei>
		static void dunode(xinxi * infor, shujuzhonglei * nodeshu, size_t weizhi) {
			char fz[dakuaisize] = { 0 };
			memdu(fz, dakuaisize, weizhi);
			memcpy(infor, fz, sizeof(xinxi));
			memcpy(nodeshu, fz + xinxisize, sizeof(shujuzhonglei));
		}
		Key fenlieyezi(size_t fzpos, xinxi & fuinfo, yezishuju & fushuju) {
			size_t fuweizhi;
			xinxi infor;
			shuju parsj;
			if (fzpos == bptr.rootpos) {
				auto rootpos = jianmid(0);
				bptr.rootpos = rootpos;
				dunode(&infor, &parsj, rootpos);
				fuinfo.fu = rootpos;
				++infor.geshu;
				parsj.a[0].erzi = fzpos;
				fuweizhi = rootpos;
			}
			else {
				dunode(&infor, &parsj, fuinfo.fu);
				fuweizhi = infor.weizhi;
			}
			if (jianchafu(fuinfo)) {
				fuweizhi = fuinfo.fu;
				dunode(&infor, &parsj, fuweizhi);
			}
			auto xinpos = jianyenode(fuweizhi, fzpos, fuinfo.next);
			auto tmppos = fuinfo.next;
			xinxi tmpxinxi;
			yezishuju tmpshuju;
			dunode(&tmpxinxi, &tmpshuju, tmppos);
			tmpxinxi.wei = xinpos;
			xienode(&tmpxinxi, &tmpshuju, tmppos);
			fuinfo.next = xinpos;
			xinxi xininfor;
			yezishuju xinshuju;
			dunode(&xininfor, &xinshuju, xinpos);
			size_t zhongpos = fuinfo.geshu / 2;
			for (size_t p = zhongpos, i = 0; p < fuinfo.geshu; ++p, ++i) {
				xinshuju.a[i].first = fushuju.a[p].first;
				xinshuju.a[i].second = fushuju.a[p].second;
				++xininfor.geshu;
			}
			fuinfo.geshu = zhongpos;
			charubz(infor, parsj, fzpos, xinpos, fushuju.a[zhongpos].first);
			xienode(&fuinfo, &fushuju, fzpos);
			xienode(&xininfor, &xinshuju, xinpos);
			xienode(&infor, &parsj, fuweizhi);
			return xinshuju.a[0].first;
		}
		bool jianchafu(xinxi & erziinfor) {
			size_t fuweizhi, frompos = erziinfor.fu;
			xinxi infor, fuinfo;
			shuju parsj, fushuju;
			dunode(&fuinfo, &fushuju, frompos);
			if (fuinfo.geshu < keygeshu)
				return false;
			if (frompos == bptr.rootpos) {
				auto rootpos = jianmid(0);
				bptr.rootpos = rootpos;
				dunode(&infor, &parsj, rootpos);
				fuinfo.fu = rootpos;
				++infor.geshu;
				parsj.a[0].erzi = frompos;
				fuweizhi = rootpos;
			}
			else {
				dunode(&infor, &parsj, fuinfo.fu);
				fuweizhi = infor.weizhi;
			}
			if (jianchafu(fuinfo)) {
				fuweizhi = fuinfo.fu;
				dunode(&infor, &parsj, fuweizhi);
			}
			auto xinpos = jianmid(fuweizhi);
			xinxi xininfor;
			shuju xinshuju;
			dunode(&xininfor, &xinshuju, xinpos);
			size_t zhongpos = fuinfo.geshu / 2;
			for (size_t p = zhongpos + 1, i = 0; p < fuinfo.geshu; ++p, ++i) {
				if (fushuju.a[p].erzi == erziinfor.weizhi) {
					erziinfor.fu = xinpos;
				}
				std::swap(xinshuju.a[i], fushuju.a[p]);
				++xininfor.geshu;
			}
			fuinfo.geshu = zhongpos + 1;
			charubz(infor, parsj, frompos, xinpos, fushuju.a[zhongpos].dkey);
			xienode(&fuinfo, &fushuju, frompos);
			xienode(&xininfor, &xinshuju, xinpos);
			xienode(&infor, &parsj, fuweizhi);
			return true;
		}
		void gaibz(size_t yfu, size_t yerzi, const Key & nkey) {
			xinxi infor;
			shuju parsj;
			dunode(&infor, &parsj, yfu);
			if (parsj.a[infor.geshu - 1].erzi == yerzi) {
				gaibz(infor.fu, yfu, nkey);
				return;
			}
			for (size_t curpos = infor.geshu - 2;; --curpos) {
				if (parsj.a[curpos].erzi == yerzi) {
					parsj.a[curpos].dkey = nkey;
					break;
				}
			}
			xienode(&infor, &parsj, yfu);
		}
		void hebingbz(xinxi & zuoinfor, shuju & zuoshuju, xinxi & rinfo, shuju & rshuju) {
			for (size_t p = zuoinfor.geshu, i = 0; i < rinfo.geshu; ++p, ++i) {
				zuoshuju.a[p] = rshuju.a[i];
			}
			zuoshuju.a[zuoinfor.geshu - 1].dkey = adjust(rinfo.fu, rinfo.weizhi);
			zuoinfor.geshu += rinfo.geshu;
			xienode(&zuoinfor, &zuoshuju, zuoinfor.weizhi);
		}
		void tiaozheng(xinxi & info, shuju & sj) {
			if (info.geshu >= keygeshu / 2) {
				xienode(&info, &sj, info.weizhi);
				return;
			}
			if (info.weizhi == bptr.rootpos && info.geshu <= 1) {
				bptr.rootpos = sj.a[0].erzi;
				return;
			}
			else if (info.weizhi == bptr.rootpos) {
				xienode(&info, &sj, info.weizhi);
				return;
			}
			xinxi infor, xdinfor;
			shuju parsj, xdshuju;
			dunode(&infor, &parsj, info.fu);
			size_t valueweizhi;
			for (valueweizhi = 0; parsj.a[valueweizhi].erzi != info.weizhi; ++valueweizhi);
			if (valueweizhi > 0) {
				dunode(&xdinfor, &xdshuju, parsj.a[valueweizhi - 1].erzi);
				xdinfor.fu = info.fu;
				if (xdinfor.geshu > keygeshu / 2) {
					for (size_t p = info.geshu; p > 0; --p) {
						sj.a[p] = sj.a[p - 1];
					}
					sj.a[0].erzi = xdshuju.a[xdinfor.geshu - 1].erzi;
					sj.a[0].dkey = parsj.a[valueweizhi - 1].dkey;
					parsj.a[valueweizhi - 1].dkey = xdshuju.a[xdinfor.geshu - 2].dkey;
					--xdinfor.geshu;
					++info.geshu;
					xienode(&xdinfor, &xdshuju, xdinfor.weizhi);
					xienode(&info, &sj, info.weizhi);
					xienode(&infor, &parsj, infor.weizhi);
					return;
				}
				else {
					hebingbz(xdinfor, xdshuju, info, sj);
					return;
				}
			}
			if (valueweizhi < infor.geshu - 1) {
				dunode(&xdinfor, &xdshuju, parsj.a[valueweizhi + 1].erzi);
				xdinfor.fu = info.fu;
				if (xdinfor.geshu > keygeshu / 2) {
					sj.a[info.geshu].erzi = xdshuju.a[0].erzi;
					sj.a[info.geshu - 1].dkey = parsj.a[valueweizhi].dkey;
					parsj.a[valueweizhi].dkey = xdshuju.a[0].dkey;
					for (size_t p = 1; p < xdinfor.geshu; ++p) {
						xdshuju.a[p - 1] = xdshuju.a[p];
					}
					--xdinfor.geshu;
					++info.geshu;
					xienode(&xdinfor, &xdshuju, xdinfor.weizhi);
					xienode(&info, &sj, info.weizhi);
					xienode(&infor, &parsj, infor.weizhi);
					return;
				}
				else {
					hebingbz(info, sj, xdinfor, xdshuju);
					return;
				}
			}
		}
		Key adjust(size_t fzpos, size_t removed_child) {
			xinxi info;
			shuju sj;
			dunode(&info, &sj, fzpos);
			size_t curpos;
			for (curpos = 0; sj.a[curpos].erzi != removed_child; ++curpos);
			Key ans = sj.a[curpos - 1].dkey;
			sj.a[curpos - 1].dkey = sj.a[curpos].dkey;
			for (; curpos < info.geshu - 1; ++curpos) {
				sj.a[curpos] = sj.a[curpos + 1];
			}
			--info.geshu;
			tiaozheng(info, sj);
			return ans;
		}
		void hebingyezi(xinxi & zuoinfor, yezishuju & zuoshuju, xinxi & rinfo, yezishuju & rshuju) {
			for (size_t p = zuoinfor.geshu, i = 0; i < rinfo.geshu; ++p, ++i) {
				zuoshuju.a[p].first = rshuju.a[i].first;
				zuoshuju.a[p].second = rshuju.a[i].second;
			}
			zuoinfor.geshu += rinfo.geshu;
			adjust(rinfo.fu, rinfo.weizhi);
			zuoinfor.next = rinfo.next;
			xinxi tmpxinxi;
			yezishuju tmpshuju;
			dunode(&tmpxinxi, &tmpshuju, rinfo.next);
			tmpxinxi.wei = zuoinfor.weizhi;
			xienode(&tmpxinxi, &tmpshuju, tmpxinxi.weizhi);
			xienode(&zuoinfor, &zuoshuju, zuoinfor.weizhi);
		}
		void pingye(xinxi & yeinff, yezishuju & yesj) {
			if (yeinff.geshu >= pairgeshu / 2) {
				xienode(&yeinff, &yesj, yeinff.weizhi);
				return;
			}
			else if (yeinff.weizhi == bptr.rootpos) {
				if (yeinff.geshu == 0) {
					xinxi tmpxinxi;
					yezishuju tmpshuju;
					dunode(&tmpxinxi, &tmpshuju, bptr.kuaitou);
					tmpxinxi.next = bptr.kuaiwei;
					xienode(&tmpxinxi, &tmpshuju, bptr.kuaitou);
					dunode(&tmpxinxi, &tmpshuju, bptr.kuaiwei);
					tmpxinxi.wei = bptr.kuaitou;
					xienode(&tmpxinxi, &tmpshuju, bptr.kuaiwei);
					return;
				}
				xienode(&yeinff, &yesj, yeinff.weizhi);
				return;
			}
			xinxi xdinfor, infor;
			yezishuju xdshuju;
			shuju parsj;
			dunode(&infor, &parsj, yeinff.fu);
			size_t midpos = 0;
			for (; midpos < infor.geshu; ++midpos) {
				if (parsj.a[midpos].erzi == yeinff.weizhi)
					break;
			}
			if (midpos > 0) {
				dunode(&xdinfor, &xdshuju, yeinff.wei);
				xdinfor.fu = yeinff.fu;
				if (xdinfor.geshu > pairgeshu / 2) {
					for (size_t p = yeinff.geshu; p > 0; --p) {
						yesj.a[p].first = yesj.a[p - 1].first;
						yesj.a[p].second = yesj.a[p - 1].second;
					}
					yesj.a[0].first = xdshuju.a[xdinfor.geshu - 1].first;
					yesj.a[0].second = xdshuju.a[xdinfor.geshu - 1].second;
					--xdinfor.geshu;
					++yeinff.geshu;
					gaibz(xdinfor.fu, xdinfor.weizhi, yesj.a[0].first);
					xienode(&xdinfor, &xdshuju, xdinfor.weizhi);
					xienode(&yeinff, &yesj, yeinff.weizhi);
					return;
				}
				else {
					hebingyezi(xdinfor, xdshuju, yeinff, yesj);
					return;
				}
			}
			if (midpos < infor.geshu - 1) {
				dunode(&xdinfor, &xdshuju, yeinff.next);
				xdinfor.fu = yeinff.fu;
				if (xdinfor.geshu > pairgeshu / 2) {
					yesj.a[yeinff.geshu].first = xdshuju.a[0].first;
					yesj.a[yeinff.geshu].second = xdshuju.a[0].second;
					for (size_t p = 1; p < xdinfor.geshu; ++p) {
						xdshuju.a[p - 1].first = xdshuju.a[p].first;
						xdshuju.a[p - 1].second = xdshuju.a[p].second;
					}
					++yeinff.geshu;
					--xdinfor.geshu;
					gaibz(yeinff.fu, yeinff.weizhi, xdshuju.a[0].first);
					xienode(&yeinff, &yesj, yeinff.weizhi);
					xienode(&xdinfor, &xdshuju, xdinfor.weizhi);
					return;
				}
				else {
					hebingyezi(yeinff, yesj, xdinfor, xdshuju);
					return;
				}
			}
		}
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
			xinxi kuaiinform;
			size_t curpos = 0;
		public:
			bool modify(const Value& value) {
				xinxi info;
				yezishuju yesj;
				dunode(&info, &yesj, kuaiinform.weizhi);
				yesj.a[curpos].second = value;
				xienode(&info, &yesj, kuaiinform.weizhi);
				return true;
			}
			iterator() {
			}
			iterator(const iterator& other) {
				cur_bptree = other.cur_bptree;
				kuaiinform = other.kuaiinform;
				curpos = other.curpos;
			}
			iterator operator++(int) {
				auto temp = *this;
				++curpos;
				if (curpos >= kuaiinform.geshu) {
					char fz[dakuaisize] = { 0 };
					memdu(fz, dakuaisize, kuaiinform.next);
					memcpy(&kuaiinform, fz, sizeof(kuaiinform));
					curpos = 0;
				}
				return temp;
			}
			iterator& operator++() {
				++curpos;
				if (curpos >= kuaiinform.geshu) {
					char fz[dakuaisize] = { 0 };
					memdu(fz, dakuaisize, kuaiinform.next);
					memcpy(&kuaiinform, fz, sizeof(kuaiinform));
					curpos = 0;
				}
				return *this;
			}
			iterator operator--(int) {
				auto temp = *this;
				if (curpos == 0) {
					char fz[dakuaisize] = { 0 };
					memdu(fz, dakuaisize, kuaiinform.wei);
					memcpy(&kuaiinform, fz, sizeof(kuaiinform));
					curpos = kuaiinform.geshu - 1;
				}
				else
					--curpos;
				return temp;
			}
			iterator& operator--() {
				if (curpos == 0) {
					char fz[dakuaisize] = { 0 };
					memdu(fz, dakuaisize, kuaiinform.wei);
					memcpy(&kuaiinform, fz, sizeof(kuaiinform));
					curpos = kuaiinform.geshu - 1;
				}
				else
					--curpos;

				return *this;
			}
			value_type operator*() const {
				if (curpos >= kuaiinform.geshu)
					throw invalid_iterator();
				char fz[dakuaisize] = { 0 };
				memdu(fz, dakuaisize, kuaiinform.weizhi);
				yezishuju yesj;
				memcpy(&yesj, fz + xinxisize, sizeof(yesj));
				value_type ires(yesj.a[curpos].first, yesj.a[curpos].second);
				return ires;
			}
			Value getValue() const {
				if (curpos >= kuaiinform.geshu)
					throw invalid_iterator();
				char fz[dakuaisize] = { 0 };
				memdu(fz, dakuaisize, kuaiinform.weizhi);
				yezishuju yesj;
				memcpy(&yesj, fz + xinxisize, sizeof(yesj));
				return yesj.a[curpos].second;
			}
			bool operator==(const iterator & rhs) const {
				return cur_bptree == rhs.cur_bptree
					&& kuaiinform.weizhi == rhs.kuaiinform.weizhi
					&& curpos == rhs.curpos;
			}
			bool operator==(const const_iterator & rhs) const {
				return kuaiinform.weizhi == rhs.kuaiinform.weizhi
					&& curpos == rhs.curpos;
			}
			bool operator!=(const iterator & rhs) const {
				return cur_bptree != rhs.cur_bptree || kuaiinform.weizhi != rhs.kuaiinform.weizhi || curpos != rhs.curpos;
			}
			bool operator!=(const const_iterator & rhs) const {
				return kuaiinform.weizhi != rhs.kuaiinform.weizhi || curpos != rhs.curpos;
			}
		};
		class const_iterator {
			friend class sjtu::BTree<Key, Value, Compare>::iterator;
			friend const_iterator sjtu::BTree<Key, Value, Compare>::cbegin() const;
			friend const_iterator sjtu::BTree<Key, Value, Compare>::cend() const;
			friend const_iterator sjtu::BTree<Key, Value, Compare>::find(const Key&) const;
		private:
			xinxi kuaiinform;
			size_t curpos = 0;
		public:
			const_iterator() {
			}
			const_iterator(const const_iterator& other) {

			}
			const_iterator(const iterator& other) {
				kuaiinform = other.kuaiinform;
				curpos = other.curpos;
			}
			const_iterator operator++(int) {
				auto temp = *this;
				++curpos;
				if (curpos >= kuaiinform.geshu) {
					char fz[dakuaisize] = { 0 };
					memdu(fz, dakuaisize, kuaiinform.next);
					memcpy(&kuaiinform, fz, sizeof(kuaiinform));
					curpos = 0;
				}
				return temp;
			}
			const_iterator& operator++() {
				++curpos;
				if (curpos >= kuaiinform.geshu) {
					char fz[dakuaisize] = { 0 };
					memdu(fz, dakuaisize, kuaiinform.next);
					memcpy(&kuaiinform, fz, sizeof(kuaiinform));
					curpos = 0;
				}
				return *this;
			}
			const_iterator operator--(int) {
				auto temp = *this;
				if (curpos == 0) {
					char fz[dakuaisize] = { 0 };
					memdu(fz, dakuaisize, kuaiinform.wei);
					memcpy(&kuaiinform, fz, sizeof(kuaiinform));
					curpos = kuaiinform.geshu - 1;
				}
				else
					--curpos;
				return temp;
			}
			const_iterator& operator--() {
				if (curpos == 0) {
					char fz[dakuaisize] = { 0 };
					memdu(fz, dakuaisize, kuaiinform.wei);
					memcpy(&kuaiinform, fz, sizeof(kuaiinform));
					curpos = kuaiinform.geshu - 1;
				}
				else
					--curpos;

				return *this;
			}
			value_type operator*() const {
				if (curpos >= kuaiinform.geshu)
					throw invalid_iterator();
				char fz[dakuaisize] = { 0 };
				memdu(fz, dakuaisize, kuaiinform.weizhi);
				yezishuju yesj;
				memcpy(&yesj, fz + xinxisize, sizeof(yesj));
				value_type ires(yesj.a[curpos].first, yesj.a[curpos].second);
				return ires;
			}
			bool operator==(const iterator & rhs) const {
				return kuaiinform.weizhi == rhs.kuaiinform.weizhi && curpos == rhs.curpos;
			}
			bool operator==(const const_iterator & rhs) const {
				return kuaiinform.weizhi == rhs.kuaiinform.weizhi && curpos == rhs.curpos;
			}
			bool operator!=(const iterator & rhs) const {
				return kuaiinform.weizhi != rhs.kuaiinform.weizhi || curpos != rhs.curpos;
			}
			bool operator!=(const const_iterator & rhs) const {
				return kuaiinform.weizhi != rhs.kuaiinform.weizhi || curpos != rhs.curpos;
			}
		};
		BTree() {
			wenjian = fopen(BPTREE_ADDRESS, "rb+");
			if (!wenjian) {
				wenjian = fopen(BPTREE_ADDRESS, "wb+");
				xiexinxi();
				auto nodeh = bptr.kuaicnt,
					nodet = bptr.kuaicnt + 1;
				bptr.kuaitou = nodeh;
				bptr.kuaiwei = nodet;
				jianyenode(0, 0, nodet);
				jianyenode(0, nodeh, 0);
				return;
			}
			char fz[dakuaisize] = { 0 };
			memdu(fz, dakuaisize, 0);
			memcpy(&bptr, fz, sizeof(bptr));
		}
		BTree(const BTree& other) {
			wenjian = fopen(BPTREE_ADDRESS, "rb+");
			bptr.kuaicnt = other.bptr.kuaicnt;
			bptr.kuaitou = other.bptr.kuaitou;
			bptr.kuaiwei = other.bptr.kuaiwei;
			bptr.rootpos = other.bptr.rootpos;
			bptr.geshu = other.bptr.geshu;
		}
		BTree& operator=(const BTree& other) {
			wenjian = fopen(BPTREE_ADDRESS, "rb+");
			bptr.kuaicnt = other.bptr.kuaicnt;
			bptr.kuaitou = other.bptr.kuaitou;
			bptr.kuaiwei = other.bptr.kuaiwei;
			bptr.rootpos = other.bptr.rootpos;
			bptr.geshu = other.bptr.geshu;
			return *this;
		}
		~BTree() {
			xiexinxi();
			fclose(wenjian);
		}
		pair<iterator, OperationResult> insert(const Key& key, const Value& value) {
			if (empty()) {
				auto rootpos = jianyenode(0, bptr.kuaitou, bptr.kuaiwei);
				xinxi tmpxinxi;
				yezishuju tmpshuju;
				dunode(&tmpxinxi, &tmpshuju, bptr.kuaitou);
				tmpxinxi.next = rootpos;
				xienode(&tmpxinxi, &tmpshuju, bptr.kuaitou);
				dunode(&tmpxinxi, &tmpshuju, bptr.kuaiwei);
				tmpxinxi.wei = rootpos;
				xienode(&tmpxinxi, &tmpshuju, bptr.kuaiwei);
				dunode(&tmpxinxi, &tmpshuju, rootpos);
				++tmpxinxi.geshu;
				tmpshuju.a[0].first = key;
				tmpshuju.a[0].second = value;
				xienode(&tmpxinxi, &tmpshuju, rootpos);
				++bptr.geshu;
				bptr.rootpos = rootpos;
				pair<iterator, OperationResult> ires(begin(), Success);
				return ires;
			}
			char fz[dakuaisize] = { 0 };
			size_t curpos = bptr.rootpos, curfu = 0;
			while (true) {
				memdu(fz, dakuaisize, curpos);
				xinxi temp;
				memcpy(&temp, fz, sizeof(temp));
				if (curfu != temp.fu) {
					temp.fu = curfu;
					memcpy(fz, &temp, sizeof(temp));
					memxie(fz, dakuaisize, curpos);
				}
				if (temp.bz) {
					break;
				}
				shuju sj;
				memcpy(&sj, fz + xinxisize, sizeof(sj));
				size_t child_pos = temp.geshu - 1;
				for (; child_pos > 0; --child_pos) {
					if (!(sj.a[child_pos - 1].dkey > key)) {
						break;
					}
				}
				curfu = curpos;
				curpos = sj.a[child_pos].erzi;
			}
			xinxi info;
			memcpy(&info, fz, sizeof(info));
			yezishuju yesj;
			memcpy(&yesj, fz + xinxisize, sizeof(yesj));
			for (size_t valueweizhi = 0;; ++valueweizhi) {
				if (valueweizhi < info.geshu && (!(yesj.a[valueweizhi].first<key || yesj.a[valueweizhi].first>key))) {
					return pair<iterator, OperationResult>(end(), Fail);
				}
				if (valueweizhi >= info.geshu || yesj.a[valueweizhi].first > key) {
					if (info.geshu >= pairgeshu) {
						auto cur_key = fenlieyezi(curpos, info, yesj);
						if (key > cur_key) {
							curpos = info.next;
							valueweizhi -= info.geshu;
							dunode(&info, &yesj, curpos);
						}
					}

					for (size_t p = info.geshu - 1; p >= valueweizhi; --p) {
						yesj.a[p + 1].first = yesj.a[p].first;
						yesj.a[p + 1].second = yesj.a[p].second;
						if (p == valueweizhi)
							break;
					}
					yesj.a[valueweizhi].first = key;
					yesj.a[valueweizhi].second = value;
					++info.geshu;
					xienode(&info, &yesj, curpos);
					iterator ans;
					ans.kuaiinform = info;
					ans.cur_bptree = this;
					ans.curpos = valueweizhi;
					++bptr.geshu;
					pair<iterator, OperationResult> to_return(ans, Success);
					return to_return;
				}
			}
			return pair<iterator, OperationResult>(end(), Fail);
		}
		OperationResult erase(const Key & key) {
			if (empty()) {
				return Fail;
			}
			char fz[dakuaisize] = { 0 };
			size_t curpos = bptr.rootpos, curfu = 0;
			while (true) {
				memdu(fz, dakuaisize, curpos);
				xinxi temp;
				memcpy(&temp, fz, sizeof(temp));
				if (curfu != temp.fu) {
					temp.fu = curfu;
					memcpy(fz, &temp, sizeof(temp));
					memxie(fz, dakuaisize, curpos);
				}
				if (temp.bz) {
					break;
				}
				shuju sj;
				memcpy(&sj, fz + xinxisize, sizeof(sj));
				size_t child_pos = temp.geshu - 1;
				for (; child_pos > 0; --child_pos) {
					if (!(sj.a[child_pos - 1].dkey > key)) {
						break;
					}
				}
				curfu = curpos;
				curpos = sj.a[child_pos].erzi;
			}

			xinxi info;
			memcpy(&info, fz, sizeof(info));
			yezishuju yesj;
			memcpy(&yesj, fz + xinxisize, sizeof(yesj));
			for (size_t valueweizhi = 0;; ++valueweizhi) {
				if (valueweizhi < info.geshu && (!(yesj.a[valueweizhi].first<key || yesj.a[valueweizhi].first>key))) {
					--info.geshu;
					for (size_t p = valueweizhi; p < info.geshu; ++p) {
						yesj.a[p].first = yesj.a[p + 1].first;
						yesj.a[p].second = yesj.a[p + 1].second;
					}
					pingye(info, yesj);
					--bptr.geshu;
					return Success;
				}
				if (valueweizhi >= info.geshu || yesj.a[valueweizhi].first > key) {
					return Fail;
				}
			}
			return Fail;
		}
		iterator begin() {
			iterator ires;
			char fz[dakuaisize] = { 0 };
			memdu(fz, dakuaisize, bptr.kuaitou);
			xinxi block_head;
			memcpy(&block_head, fz, sizeof(block_head));
			ires.kuaiinform = block_head;
			ires.cur_bptree = this;
			ires.curpos = 0;
			++ires;
			return ires;
		}
		const_iterator cbegin() const {
			const_iterator ires;
			char fz[dakuaisize] = { 0 };
			memdu(fz, dakuaisize, bptr.kuaitou);
			xinxi block_head;
			memcpy(&block_head, fz, sizeof(block_head));
			ires.kuaiinform = block_head;
			ires.curpos = 0;
			++ires;
			return ires;
		}
		iterator end() {
			iterator ires;
			char fz[dakuaisize] = { 0 };
			memdu(fz, dakuaisize, bptr.kuaiwei);
			xinxi block_head;
			memcpy(&block_head, fz, sizeof(block_head));
			ires.kuaiinform = block_head;
			ires.cur_bptree = this;
			ires.curpos = 0;
			return ires;
		}
		const_iterator cend() const {
			const_iterator ires;
			char fz[dakuaisize] = { 0 };
			memdu(fz, dakuaisize, bptr.kuaiwei);
			xinxi block_head;
			memcpy(&block_head, fz, sizeof(block_head));
			ires.kuaiinform = block_head;
			ires.curpos = 0;
			return ires;
		}
		bool empty() const {
			if (!wenjian)
				return true;
			return bptr.geshu == 0;
		}
		size_t size() const {
			if (!wenjian)
				return 0;
			return bptr.geshu;
		}
		void clear() {
			if (!wenjian)
				return;
			remove(BPTREE_ADDRESS);
			ffile new_file_head;
			bptr = new_file_head;
			wenjian = nullptr;
		}
		Value at(const Key & key) {
			if (empty()) {
				throw container_is_empty();
			}
			char fz[dakuaisize] = { 0 };
			size_t curpos = bptr.rootpos, curfu = 0;
			while (true) {
				memdu(fz, dakuaisize, curpos);
				xinxi temp;
				memcpy(&temp, fz, sizeof(temp));
				if (curfu != temp.fu) {
					temp.fu = curfu;
					memcpy(fz, &temp, sizeof(temp));
					memxie(fz, dakuaisize, curpos);
				}
				if (temp.bz) {
					break;
				}
				shuju sj;
				memcpy(&sj, fz + xinxisize, sizeof(sj));
				size_t child_pos = temp.geshu - 1;
				for (; child_pos > 0; --child_pos) {
					if (!(sj.a[child_pos - 1].dkey > key)) {
						break;
					}
				}
				curfu = curpos;
				curpos = sj.a[child_pos].erzi;
			}
			xinxi info;
			memcpy(&info, fz, sizeof(info));
			yezishuju yesj;
			memcpy(&yesj, fz + xinxisize, sizeof(yesj));
			for (size_t valueweizhi = 0;; ++valueweizhi) {
				if (valueweizhi < info.geshu && (!(yesj.a[valueweizhi].first<key || yesj.a[valueweizhi].first>key))) {
					return yesj.a[valueweizhi].second;
				}
				if (valueweizhi >= info.geshu || yesj.a[valueweizhi].first > key) {
					throw index_out_of_bound();
				}
			}
		}
		size_t count(const Key & key) const {
			return find(key) == cend() ? 0 : 1;
		}
		iterator find(const Key & key) {
			if (empty()) {
				return end();
			}
			char fz[dakuaisize] = { 0 };
			size_t curpos = bptr.rootpos, curfu = 0;
			while (true) {
				memdu(fz, dakuaisize, curpos);
				xinxi temp;
				memcpy(&temp, fz, sizeof(temp));
				if (curfu != temp.fu) {
					temp.fu = curfu;
					memcpy(fz, &temp, sizeof(temp));
					memxie(fz, dakuaisize, curpos);
				}
				if (temp.bz) {
					break;
				}
				shuju sj;
				memcpy(&sj, fz + xinxisize, sizeof(sj));
				size_t child_pos = temp.geshu - 1;
				for (; child_pos > 0; --child_pos) {
					if (!(sj.a[child_pos - 1].dkey > key)) {
						break;
					}
				}
				curfu = curpos;
				curpos = sj.a[child_pos].erzi;
			}
			xinxi info;
			memcpy(&info, fz, sizeof(info));
			sizeof(shuju);
			yezishuju yesj;
			memcpy(&yesj, fz + xinxisize, sizeof(yesj));
			for (size_t valueweizhi = 0;; ++valueweizhi) {
				if (valueweizhi < info.geshu && (!(yesj.a[valueweizhi].first<key || yesj.a[valueweizhi].first>key))) {
					iterator ires;
					ires.cur_bptree = this;
					ires.kuaiinform = info;
					ires.curpos = valueweizhi;
					return ires;
				}
				if (valueweizhi >= info.geshu || yesj.a[valueweizhi].first > key) {
					return end();
				}
			}
			return end();
		}
		const_iterator find(const Key & key) const {
			if (empty()) {
				return cend();
			}
			char fz[dakuaisize] = { 0 };
			size_t curpos = bptr.rootpos, curfu = 0;
			while (true) {
				memdu(fz, dakuaisize, curpos);
				xinxi temp;
				memcpy(&temp, fz, sizeof(temp));
				if (curfu != temp.fu) {
					temp.fu = curfu;
					memcpy(fz, &temp, sizeof(temp));
					memxie(fz, dakuaisize, curpos);
				}
				if (temp.bz) {
					break;
				}
				shuju sj;
				memcpy(&sj, fz + xinxisize, sizeof(sj));
				size_t child_pos = temp.geshu - 1;
				for (; child_pos > 0; --child_pos) {
					if (!(sj.a[child_pos - 1].dkey > key)) {
						break;
					}
				}
				curfu = curpos;
				curpos = sj.a[child_pos].erzi;
			}
			xinxi info;
			memcpy(&info, fz, sizeof(info));
			yezishuju yesj;
			memcpy(&yesj, fz + xinxisize, sizeof(yesj));
			for (size_t valueweizhi = 0;; ++valueweizhi) {
				if (valueweizhi < info.geshu && (!(yesj.a[valueweizhi].first<key || yesj.a[valueweizhi].first>key))) {
					const_iterator ires;
					ires.kuaiinform = info;
					ires.curpos = valueweizhi;
					return ires;
				}
				if (valueweizhi >= info.geshu || yesj.a[valueweizhi].first > key) {
					return cend();
				}
			}
			return cend();
		}
	};
	template <typename Key, typename Value, typename Compare> FILE* BTree<Key, Value, Compare>::wenjian = nullptr;
}