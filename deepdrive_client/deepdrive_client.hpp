#pragma once

#include <zmq.hpp>
#include <rapidjson/document.h>

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

    /** Get Deepdrive observation after executing value **/
    rapidjson::Value step(rapidjson::Value action);


  protected:

  private:
    zmq::context_t _context;
    zmq::socket_t _socket;

    void send_start_message();

    rapidjson::Document
    send(rapidjson::Value &method, rapidjson::Value &args, rapidjson::Value &kwargs);
  };

}  // namespace deepdrive


/*
  def deserialize_space(resp):
      if resp['type'] == "<class 'gym.spaces.box.Box'>":
          ret = spaces.Box(resp['low'], resp['high'], dtype=resp['dtype'])
      else:
          raise RuntimeError('Unsupported action space type')
      return ret


  class Client(object):
      """
      A Client object acts as a remote proxy to the deepdrive gym environment.
      Methods that you would call on the env, like step() are also called on
      this object, with communication over the network -
      rather than over shared memory (for observations) and network
      (for transactions like reset) as is the case with the locally run
      sim/gym_env.py.
      This allows the agent and environment to run on separate machines, but
      with the same API as a local agent, namely the gym API.
      The local gym environment is then run by api/server.py which proxies
      RPC's from this client to the local environment.
      All network communication happens over ZMQ to take advantage of their
      highly optimized cross-language / cross-OS sockets.
      NOTE: This will obviously run more slowly than a local agent which
      communicates sensor data over shared memory.
      """
      def __init__(self, **kwargs):
          """
          :param kwargs['client_render'] (bool): Whether to render on this
              side of the client server connection.
              Passing kwargs['render'] = True will cause the server to render
              an MJPG stream at http://localhost:5000
          """
          self.socket = None
          self.last_obz = None
          self.create_socket()
          self.should_render = kwargs.get('client_render', False)
          kwargs['cameras'] = kwargs.get('cameras', [c.DEFAULT_CAM])
          log.info('Waiting for sim to start on server...')
          # TODO: Fix connecting to an open sim
          self._send(m.START, kwargs=kwargs)
          log.info('===========> Deepdrive sim started')

      def step(self, action):
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