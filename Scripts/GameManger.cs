using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManger : MonoBehaviour
{
    public float timeScale;

    public bool ismove;

    public GameObject gameWinPanel;
    public GameObject gameOverPanel;

    public GameObject hpImage1;
    public GameObject hpImage2;
    public GameObject hpImage3;

    public int hp;
    public int blockNum;

    public GameObject ballprefab;
    GameObject _gameover;
    Timer timer;

    // Start is called before the first frame update
    void Start()
    {
        hp = 3;
        blockNum = 16;
        Time.timeScale = 0f;
        _gameover = GameObject.FindWithTag("DeadZone");
        timer = GetComponent<Timer>();
    }

    // Update is called once per frame
    void Update()
    {
        ballmake();

        if (Input.GetKey(KeyCode.Space))
        {
            Time.timeScale = 1.0f;
        }

        if (hp == 2)
        {
            hpImage3.SetActive(false);
        }
        if (hp == 1)
        {
            hpImage2.SetActive(false);
        }
        if (hp == 0)
        {
            hpImage1.SetActive(false);
        }
        Win();
        Over();
    }

    void Win()
    {
        if(blockNum == 0)
        {
            Time.timeScale = 0f;
            gameWinPanel.SetActive(true);
        }
    }

    void Over()
    {
        if (hp == 0 || timer.GetComponent<Timer>().setTime == 0)
        {
            Time.timeScale = 0f;
            gameOverPanel.SetActive(true);
        }
    }

    public void ballmake()
    {
        if (_gameover.GetComponent<DeadZone>().isball == false && hp > 0)
        {
            GameObject newball = Instantiate(ballprefab);
            _gameover.GetComponent<DeadZone>().isball = true;
        }
    }
}
