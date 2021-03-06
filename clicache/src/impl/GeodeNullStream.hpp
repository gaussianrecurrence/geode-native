/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once


#include "../geode_defs.hpp"

using namespace System;
using namespace System::IO;

namespace Apache
{
  namespace Geode
  {
    namespace Client
    {

      ref class GeodeNullStream : public Stream
      {
      public:

        virtual property bool CanSeek { bool get() override { return false; } }
        virtual property bool CanRead { bool get() override { return false; } }
        virtual property bool CanWrite { bool get() override { return true; } }

        virtual void Close() override { Stream::Close(); }

        virtual property System::Int64 Length
        {
          System::Int64 get() override
          {
            return (System::Int64) m_position;
          }
        }

        virtual property System::Int64 Position
        {
          System::Int64 get() override
          {
            return (System::Int64) m_position;
          }

          void set(System::Int64 value) override
          {
            m_position = (int) value;
          }
        }

        virtual System::Int64 Seek(System::Int64 offset, SeekOrigin origin) override
        {
          throw gcnew System::NotSupportedException("Seek not supported by GeodeNullStream");
          /*
          int actual = 0;
          switch (origin)
          {
          case SeekOrigin::Begin:
            actual = (int) offset;
            m_position = (int) actual;
            break;

          case SeekOrigin::Current:
            actual = (int) offset;
            m_position += (int) actual;
            break;

          case SeekOrigin::End:
            actual = (int) offset;
            m_position += (int) actual;
            break;
          }
          // Seek is meaningless here?
          return m_position;
          */
        }

        virtual void SetLength(System::Int64 value) override { /* do nothing */ }

        virtual void Write(array<Byte> ^ buffer, int offset, int count) override
        {
          m_position += count;
        }

        virtual void WriteByte(unsigned char value) override
        {
          m_position++;
        }

        virtual int Read(array<Byte> ^ buffer, int offset, int count) override
        {
          throw gcnew System::NotSupportedException("Seek not supported by GeodeNullStream");
          /*
          int actual = count;
          m_position += actual;
          return actual;
          */
        }

        virtual void Flush() override { /* do nothing */ }

      private:
        int m_position;
      };
    }  // namespace Client
  }  // namespace Geode
}  // namespace Apache
