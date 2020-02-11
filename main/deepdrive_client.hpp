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

    DDOut step();


  protected:

  private:

  };

}  // namespace deepdrive


/*
 *     def step(self, action):
        if hasattr(action, 'as_gym'):
            # Legacy support for original agents written within deepdrive repo
            action = action.as_gym()
        obz, reward, done, info = self._send(m.STEP, args=[action])
        if not obz:
            obz = None
        self.last_obz = obz
        if self.should_render:
            self.render()
        return obz, reward, done, info

    def reset(self):
        return self._send(m.RESET)

    def render(self):
        """
        We pass the obz through an instance variable to comply with
        the gym api where render() takes 0 arguments
        """
        if self.last_obz is not None:
            self.renderer.render(self.last_obz)

    def change_cameras(self, cameras):
        return self._send(m.CHANGE_CAMERAS, args=[cameras])

    def close(self):
        self._send(m.CLOSE)
        try:
            self.socket.close()
        except Exception as e:
            log.debug('Caught exception closing socket')

    @property
    def action_space(self):
        resp = self._send(m.ACTION_SPACE)
        ret = deserialize_space(resp)
        return ret

    @property
    def observation_space(self):
        resp = self._send(m.OBSERVATION_SPACE)
        ret = deserialize_space(resp)
        return ret

    @property
    def metadata(self):
        return self._send(m.METADATA)

    @property
    def reward_range(self):
        return self._send(m.REWARD_RANGE)


def get_action(steering=0, throttle=0, brake=0, handbrake=0, has_control=True):
    ret = [np.array([steering]),
           np.array([throttle]),
           np.array([brake]),
           np.array([handbrake]),
           has_control]
    return ret
 */