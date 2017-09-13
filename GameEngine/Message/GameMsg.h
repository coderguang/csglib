#ifndef _MESSAGE_GAME_MSG_H_
#define _MESSAGE_GAME_MSG_H_

#include "../../Engine/engine/serialize/serializestream.h"
#include "../../Engine/framework/log/Log.h"
#include "../../Engine/framework/util/typetransform.h"
#include "../../Engine/engine/def/csg_def.h"
#include "../../Engine/engine/mq/msgBlock.h"
#include "../../Engine/engine/mq/msgManager.h"

using namespace csg;
namespace Message
{
	class SeriaInt
	{
	public:
		int aInt;

		void read(CSerializeStream& os)
		{
			os.read(aInt);
		}

		void write(CSerializeStream& os)
		{
			os.write(aInt);
		}

		void print()
		{
			CSG_LOG_DEBUG("aInt=" << ToStr(aInt));
		}
	};


	class SeriaTestAllTest
	{
	public:
		byte_t bByte;
		std::vector<byte_t> bBytes;
		int iInt;
		std::vector<int> iInts;
		bool bBool;
		std::vector<bool> bBools;
		short sShort;
		std::vector<short> sShorts;
		long64_t lLong;
		std::vector<long64_t> lLongs;
		std::string str;
		std::vector<std::string> strs;

		void read(CSerializeStream& os)
		{
			os.read(bByte);
			os.read(bBytes);
			os.read(iInt);
			os.read(iInts);
			os.read(bBool);
			os.read(bBools);
			os.read(sShort);
			os.read(sShorts);
			os.read(lLong);
			os.read(lLongs);
			os.read(str);
			os.read(strs);
		}

		void write(CSerializeStream& os)
		{
			os.write(bByte);
			os.write(bBytes);
			os.write(iInt);
			os.write(iInts);
			os.write(bBool);
			os.write(bBools);
			os.write(sShort);
			os.write(sShorts);
			os.write(lLong);
			os.write(lLongs);
			os.write(str);
			os.write(strs);
		}

		bool operator == ( const SeriaTestAllTest& other )
		{
			if ( bByte != other.bByte )
			{
				return false;
			}
			if ( iInt != other.iInt )
			{
				return false;
			}
			if ( bBool != other.bBool )
			{
				return false;
			}
			if ( sShort != other.sShort )
			{
				return false;
			}
			if ( lLong != other.lLong )
			{
				return false;
			}
			if ( str != other.str )
			{
				return false;
			}

			if ( bBytes.size() != other.bBytes.size() )
			{
				return false;
			}
			for ( int i = 0; i < bBytes.size(); i++ )
			{
				if ( bBytes[i] != other.bBytes[i] )
				{
					return false;
				}
			}

			if ( iInts.size() != other.iInts.size() )
			{
				return false;
			}
			for ( int i = 0; i < iInts.size(); i++ )
			{
				if ( iInts[i] != other.iInts[i] )
				{
					return false;
				}
			}

			if ( bBools.size() != other.bBools.size() )
			{
				return false;
			}
			for ( int i = 0; i < bBools.size(); i++ )
			{
				if ( bBools[i] != other.bBools[i] )
				{
					return false;
				}
			}

			if ( sShorts.size() != other.sShorts.size() )
			{
				return false;
			}
			for ( int i = 0; i < sShorts.size(); i++ )
			{
				if ( sShorts[i] != other.sShorts[i] )
				{
					return false;
				}
			}

			if ( lLongs.size() != other.lLongs.size() )
			{
				return false;
			}
			for ( int i = 0; i < lLongs.size(); i++ )
			{
				if ( lLongs[i] != other.lLongs[i] )
				{
					return false;
				}
			}

			if ( strs.size() != other.strs.size() )
			{
				return false;
			}
			for ( int i = 0; i < strs.size(); i++ )
			{
				if ( strs[i] != other.strs[i] )
				{
					return false;
				}
			}

			return true;
		}

		void print()
		{
			CSG_LOG_DEBUG("bByte=" << ToStr(bByte));
			CSG_LOG_DEBUG("iInt=" << ToStr(iInt));
			CSG_LOG_DEBUG("bBool=" << ToStr(bBool));
			CSG_LOG_DEBUG("sShort=" << ToStr(sShort));
			CSG_LOG_DEBUG("lLong=" << ToStr(lLong));
			CSG_LOG_DEBUG("str=" << str);
			

			for ( int i = 0; i < bBytes.size();i++ )
				CSG_LOG_DEBUG("bBytes" << ToStr(i)<<"="<<ToStr(bBytes[i]));

			for ( int i = 0; i < iInts.size(); i++ )
				CSG_LOG_DEBUG("iInts" << ToStr(i) << "=" << ToStr(iInts[i]));

			for ( int i = 0; i < sShorts.size(); i++ )
				CSG_LOG_DEBUG("sShorts" << ToStr(i) << "=" << ToStr(sShorts[i]));

			for ( int i = 0; i < lLongs.size(); i++ )
				CSG_LOG_DEBUG("lLongs" << ToStr(i) << "=" << ToStr(lLongs[i]));

			for ( int i = 0; i < strs.size(); i++ )
				CSG_LOG_DEBUG("strs" << ToStr(i) << "=" << strs[i]);
		}


		void randValue()
		{
			bByte = rand()%255;
			iInt = 2;
			bBool = true;
			sShort = 4;
			lLong = 5;
			str = "6";
			int mod = 20;
			int randNum = rand() % 5;
			for ( int i = 0; i < randNum; i++ )
			{
				bBytes.push_back(i);
			}
			randNum = rand() % mod;
			
			for ( int i = 0; i < randNum; i++ )
			{
				iInts.push_back(i);
			}
			randNum = rand() % mod;
			
			for ( int i = 0; i < randNum; i++ )
			{
				bBools.push_back(i % 2);
			}
			randNum = rand() % mod;
			
			for ( int i = 0; i < randNum; i++ )
			{
				sShorts.push_back(i);
			}
			for ( int i = 0; i < 100; i++ )
			{
				randNum = rand();
				lLongs.push_back(randNum);
			}
			randNum = rand() % mod;
			for ( int i = 0; i < randNum; i++ )
			{
				strs.push_back("hello");
			}
		}
	};


	class SeriaIntEx:public virtual csg::IMsgBase
	{
	public:
		int aInt;

		void _csg_read(CSerializeStream& os)
		{
			os.read(aInt);
		}

		void _csg_write(CSerializeStream& os)const
		{
			os.write(aInt);
		}

		void _csg_init()
		{
			aInt = 0;
		}

		IMsgBase* clone()
		{
			return new SeriaIntEx(*this);
		};

		int getType()const
		{
			return 1;
		}

		void print()
		{
			CSG_LOG_DEBUG("aInt=" << ToStr(aInt));
		}

		SeriaIntEx()
		{
			_csg_init();
		}

		SeriaIntEx(const SeriaIntEx& other)
		{
			aInt = other.aInt;
		}

		SeriaIntEx& operator=( const SeriaIntEx& other )
		{
			aInt = other.aInt;
		}
	};

	typedef csg::CSmartPointShare<SeriaIntEx> SeriaIntExPtr;


	class SeriaTestAllTestEx :public virtual csg::IMsgBase
	{
	public:
		SeriaTestAllTestEx()
		{
			_csg_init();
		}
		SeriaTestAllTestEx(const SeriaTestAllTestEx& other)
		{

		}

		SeriaTestAllTestEx& operator=( const SeriaTestAllTestEx& other )
		{
			
		}

		byte_t bByte;
		std::vector<byte_t> bBytes;
		int iInt;
		std::vector<int> iInts;
		bool bBool;
		std::vector<bool> bBools;
		short sShort;
		std::vector<short> sShorts;
		long64_t lLong;
		std::vector<long64_t> lLongs;
		std::string str;
		std::vector<std::string> strs;

		void _csg_read(CSerializeStream& os)
		{
			os.read(bByte);
			os.read(bBytes);
			os.read(iInt);
			os.read(iInts);
			os.read(bBool);
			os.read(bBools);
			os.read(sShort);
			os.read(sShorts);
			os.read(lLong);
			os.read(lLongs);
			os.read(str);
			os.read(strs);
		}

		void _csg_write(CSerializeStream& os)const
		{
			os.write(bByte);
			os.write(bBytes);
			os.write(iInt);
			os.write(iInts);
			os.write(bBool);
			os.write(bBools);
			os.write(sShort);
			os.write(sShorts);
			os.write(lLong);
			os.write(lLongs);
			os.write(str);
			os.write(strs);
		}

		void print()
		{
			CSG_LOG_DEBUG("bByte=" << ToStr(bByte));
			CSG_LOG_DEBUG("iInt=" << ToStr(iInt));
			CSG_LOG_DEBUG("bBool=" << ToStr(bBool));
			CSG_LOG_DEBUG("sShort=" << ToStr(sShort));
			CSG_LOG_DEBUG("lLong=" << ToStr(lLong));
			CSG_LOG_DEBUG("str=" << str);


			for ( int i = 0; i < bBytes.size(); i++ )
				CSG_LOG_DEBUG("bBytes" << ToStr(i) << "=" << ToStr(bBytes[i]));

			for ( int i = 0; i < iInts.size(); i++ )
				CSG_LOG_DEBUG("iInts" << ToStr(i) << "=" << ToStr(iInts[i]));

			for ( int i = 0; i < sShorts.size(); i++ )
				CSG_LOG_DEBUG("sShorts" << ToStr(i) << "=" << ToStr(sShorts[i]));

			for ( int i = 0; i < lLongs.size(); i++ )
				CSG_LOG_DEBUG("lLongs" << ToStr(i) << "=" << ToStr(lLongs[i]));

			for ( int i = 0; i < strs.size(); i++ )
				CSG_LOG_DEBUG("strs" << ToStr(i) << "=" << strs[i]);
		}


		void _csg_init()
		{
			bByte = rand() % 255;
			iInt = 2;
			bBool = true;
			sShort = 4;
			lLong = 5;
			str = "6";
			int mod = 20;
			int randNum = rand() % 5;
			for ( int i = 0; i < randNum; i++ )
			{
				bBytes.push_back(i);
			}
			randNum = rand() % mod;

			for ( int i = 0; i < randNum; i++ )
			{
				iInts.push_back(i);
			}
			randNum = rand() % mod;

			for ( int i = 0; i < randNum; i++ )
			{
				bBools.push_back(i % 2);
			}
			randNum = rand() % mod;

			for ( int i = 0; i < randNum; i++ )
			{
				sShorts.push_back(i);
			}
			for ( int i = 0; i < 100; i++ )
			{
				randNum = rand();
				lLongs.push_back(randNum);
			}
			randNum = rand() % mod;
			for ( int i = 0; i < randNum; i++ )
			{
				strs.push_back("hello");
			}
		}

		IMsgBase* clone()
		{
			return new SeriaTestAllTestEx(*this);
		};

		int getType()const
		{
			return 2;
		}

		void randValue()
		{
			bByte = rand() % 255;
			iInt = 2;
			bBool = true;
			sShort = 4;
			lLong = 5;
			str = "6";
			int mod = 20;
			int randNum = rand() % 5;
			for ( int i = 0; i < randNum; i++ )
			{
				bBytes.push_back(i);
			}
			randNum = rand() % mod;

			for ( int i = 0; i < randNum; i++ )
			{
				iInts.push_back(i);
			}
			randNum = rand() % mod;

			for ( int i = 0; i < randNum; i++ )
			{
				bBools.push_back(i % 2);
			}
			randNum = rand() % mod;

			for ( int i = 0; i < randNum; i++ )
			{
				sShorts.push_back(i);
			}
			for ( int i = 0; i < 100; i++ )
			{
				randNum = rand();
				lLongs.push_back(randNum);
			}
			randNum = rand() % mod;
			for ( int i = 0; i < randNum; i++ )
			{
				strs.push_back("hello");
			}
		}
	};

	typedef csg::CSmartPointShare<SeriaTestAllTestEx> SeriaTestAllTestExPtr;

	class CGateMsg
	{
	public:
		CGateMsg()
		{
			regist();
		}
		void regist()
		{
			csg::CMsgManager::instance()->regist(new SeriaIntEx());
			csg::CMsgManager::instance()->regist(new SeriaTestAllTestEx());
		}
	};


	//RPC≤‚ ‘
	class ITest
	{
		

	};


}
#endif