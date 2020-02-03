#pragma once

void hello();

namespace deepdrive {
/**
 * ZMQ client for the Deepdrive simulator
 */
class DeepdriveClient {
  public:
    /**
     * Contructs a new client.
     *
     */
    explicit DeepdriveClient();

    virtual ~DeepdriveClient();



  protected:

  private:

};
}  // namespace deepdrive