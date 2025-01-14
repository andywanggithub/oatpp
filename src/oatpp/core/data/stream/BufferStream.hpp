/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#ifndef oatpp_data_stream_BufferStream_hpp
#define oatpp_data_stream_BufferStream_hpp

#include "Stream.hpp"

namespace oatpp { namespace data{ namespace stream {

/**
 * BufferOutputStream
 */
class BufferOutputStream : public ConsistentOutputStream {
private:
  p_char8 m_data;
  v_io_size m_capacity;
  v_io_size m_position;
  v_io_size m_growBytes;
  IOMode m_ioMode;
public:

  /**
   * Constructor.
   * @param growBytes
   */
  BufferOutputStream(v_io_size initialCapacity = 2048, v_io_size growBytes = 2048);

  /**
   * Virtual destructor.
   */
  ~BufferOutputStream();

  /**
   * Write `count` of bytes to stream.
   * @param data - data to write.
   * @param count - number of bytes to write.
   * @return - actual number of bytes written. &id:oatpp::data::v_io_size;.
   */
  data::v_io_size write(const void *data, data::v_io_size count) override;

  /**
   * Set stream I/O mode.
   * @throws
   */
  void setOutputStreamIOMode(IOMode ioMode) override;

  /**
   * Get stream I/O mode.
   * @return
   */
  IOMode getOutputStreamIOMode() override;

  /**
   * Reserve bytes for future writes.
   */
  void reserveBytesUpfront(v_io_size count);

  /**
   * Get pointer to data.
   * @return - pointer to data.
   */
  p_char8 getData();

  /**
   * Get current capacity.
   * Capacity may change.
   * @return
   */
  v_io_size getCapacity();

  /**
   * Get current data write position.
   * @return - current data write position.
   */
  v_io_size getCurrentPosition();

  /**
   * Set current data write position.
   * @param position - data write position.
   */
  void setCurrentPosition(v_io_size position);

  /**
   * Copy data to &id:oatpp::String;.
   * @return
   */
  oatpp::String toString();

  /**
   * Create &id:oatpp::String; from part of buffer.
   * @param pos - starting position in buffer.
   * @param count - size of bytes to write to substring.
   * @return - &id:oatpp::String;
   */
  oatpp::String getSubstring(data::v_io_size pos, data::v_io_size count);

  /**
   * Write all bytes from buffer to stream.
   * @param stream - stream to flush all data to.
   * @return - actual amount of bytes flushed.
   */
  oatpp::data::v_io_size flushToStream(OutputStream* stream);

  /**
   * Write all bytes from buffer to stream in async manner.
   * @param _this - pointer to `this` buffer.
   * @param stream - stream to flush all data to.
   * @return - &id:oatpp::async::CoroutineStarter;.
   */
  static oatpp::async::CoroutineStarter flushToStreamAsync(const std::shared_ptr<BufferOutputStream>& _this, const std::shared_ptr<OutputStream>& stream);

};

/**
 * BufferInputStream
 */
class BufferInputStream : public InputStream {
private:
  std::shared_ptr<base::StrBuffer> m_memoryHandle;
  p_char8 m_data;
  v_io_size m_size;
  v_io_size m_position;
  IOMode m_ioMode;
public:

  /**
   * Constructor.
   * @param memoryHandle - buffer memory handle. May be nullptr.
   * @param data - pointer to buffer data.
   * @param size - size of the buffer.
   */
  BufferInputStream(const std::shared_ptr<base::StrBuffer>& memoryHandle, p_char8 data, v_io_size size);

  /**
   * Constructor.
   * @param data - buffer.
   */
  BufferInputStream(const oatpp::String& data);

  /**
   * Reset stream data and set position to `0`.
   * @param memoryHandle - buffer memory handle. May be nullptr.
   * @param data - pointer to buffer data.
   * @param size - size of the buffer.
   */
  void reset(const std::shared_ptr<base::StrBuffer>& memoryHandle, p_char8 data, v_io_size size);


  /**
   * Same as `reset(nullptr, nullptr, 0);.`
   */
  void reset();

  /**
   * Read data from stream. <br>
   * It is a legal case if return result < count. Caller should handle this!
   * *Calls to this method are always NON-BLOCKING*
   * @param data - buffer to read data to.
   * @param count - size of the buffer.
   * @return - actual number of bytes read. 0 - designates end of the buffer.
   */
  data::v_io_size read(void *data, data::v_io_size count) override;

  /**
   * In case of a `BufferInputStream` suggested Action is always &id:oatpp::async::Action::TYPE_REPEAT; if `ioResult` is greater then zero. <br>
   * @param ioResult - result of the call to &l:BufferInputStream::read ();.
   * @return - &id:oatpp::async::Action;.
   * @throws - `std::runtime_error` if ioResult <= 0.
   */
  oatpp::async::Action suggestInputStreamAction(data::v_io_size ioResult) override;

  /**
   * Set stream I/O mode.
   * @throws
   */
  void setInputStreamIOMode(IOMode ioMode) override;

  /**
   * Get stream I/O mode.
   * @return
   */
  IOMode getInputStreamIOMode() override;

  /**
   * Get data memory handle.
   * @return - data memory handle.
   */
  std::shared_ptr<base::StrBuffer> getDataMemoryHandle();

  /**
   * Get pointer to data.
   * @return - pointer to data.
   */
  p_char8 getData();

  /**
   * Get data size.
   * @return - data size.
   */
  v_io_size getDataSize();

  /**
   * Get current data read position.
   * @return - current data read position.
   */
  v_io_size getCurrentPosition();

  /**
   * Set current data read position.
   * @param position - data read position.
   */
  void setCurrentPosition(v_io_size position);


};

}}}

#endif // oatpp_data_stream_BufferStream_hpp
