import React, { useState, useEffect } from "react";
import VeryGoodPic from "../assets/best.png";
import GoodPic from "../assets/good.png";
import ModeratePic from "../assets/normal.png";
import ImpactPic from "../assets/bad.png";
import ExtremelyPic from "../assets/worst.png";
import { FaCheckCircle } from "react-icons/fa";
import LoadingGif from "../assets/loading.gif";

import Snackbar from "@mui/joy/Snackbar";
import Button from "@mui/joy/Button";
import CircularProgress from "@mui/joy/CircularProgress";

function Detail({ data }) {
  if (!data) {
    return <p>No data available</p>;
  }

  const { humidity, temp, dust } = data;
  const [state, setState] = useState(0);
  const [isLoading, setIsloading] = useState(false);
  const [isComplete, setIsComplete] = useState(false);

  // test
  const [open, setOpen] = React.useState(false);
  //const [variant, setVariant] = React.useState<SnackbarProps['variant']>('outlined');

  const handleOpenMachine = () => {
    setIsComplete(false);
    setIsloading(true);
    setTimeout(() => {
      setIsComplete(true);
      setIsloading(false);
    }, 3000);
  };
  const qualityLevels = [
    {
      label: "Very Good",
      color: "bg-cyan-400",
      icon: VeryGoodPic,
      hasToOpen: false,
      description: [
        "The PM 2.5 dust level is between 0-25 µg/m³.",
        "The air quality is good",
      ],
      hasWarning: false,
      warning: " suitable for outdoor activities and tourism.",
    },
    {
      label: "Good",
      color: "bg-lime-400",
      icon: GoodPic,
      hasToOpen: false,
      description: [
        "The PM 2.5 dust level is between 26-37 µg/m³.",
        "The air quality is fair",
      ],
      hasWarning: false,
      warning: "allowing for outdoor activities and tourism as usual.",
    },
    {
      label: "Moderate",
      color: "bg-yellow-400",
      icon: ModeratePic,
      hasToOpen: true,
      description: [
        "The PM 2.5 dust level is between 38-50 µg/m³.",
        "The air quality is moderate.",
      ],
      hasWarning: true,
      warning:
        "the general public can carry out outdoor activities as usual. individuals with special health concerns, such as those experiencing initial symptoms like coughing, difficulty breathing, or eye irritation, should reduce the duration of outdoor activities.",
    },
    {
      label: "Very Poor",
      color: "bg-orange-400",
      icon: ImpactPic,
      hasToOpen: true,
      description: [
        "The PM 2.5 dust level is between 51-90 µg/m³.",
        "The air quality is starting to have health impacts.",
      ],
      hasWarning: true,
      warning:
        "The general public should be cautious about their health. If experiencing initial symptoms such as coughing, difficulty breathing, or eye irritation, it's advisable to reduce outdoor activity duration or use protective equipment if necessary.",
    },
    {
      label: "Extremely Poor",
      color: "bg-red-400",
      icon: ExtremelyPic,
      hasToOpen: true,
      description: [
        "The PM 2.5 dust level is at 91 µg/m³ or higher.",
        "The air quality is at a level where it significantly impacts health.",
      ],
      hasWarning: true,
      warning:
        " Everyone should avoid outdoor activities, stay away from areas with high air pollution, or use protective equipment if necessary. If experiencing health symptoms, it's important to consult a doctor.",
    },
  ];

  useEffect(() => {
    let newState = 0;

    if (dust <= 25) {
      newState = 0;
    } else if (dust <= 37) {
      newState = 1;
    } else if (dust <= 50) {
      newState = 2;
    } else if (dust <= 90) {
      newState = 3;
    } else {
      newState = 4;
    }

    // Update state only if the new state is different from the current state
    if (newState !== state) {
      setState(newState);
    }
  }, [dust]); // Trigger effect when 'dust' value changes

  return (
    <div className="w-6/12 mx-2 items-center">
      <div className={`container flex flex-col justify-between  items-center`}>
        <div className={`flex flex-row w-full  p-2 items-center `}>
          <img
            className="w-40 h-40 mr-4"
            src={qualityLevels[state].icon}
            alt={qualityLevels[state].label}
          />
          <div className="flex flex-col items-center justify-items-center">
            <p
              className={`text-white ${qualityLevels[state].color} w-10/12 p-2 rounded-full font-bold text-xl`}
            >
              Level: {qualityLevels[state].label}
            </p>
            <ul className=" py-2 w-[21.5em]">
              {qualityLevels[state].description.map((message, index) => (
                <li key={index}>
                  <p>{message}</p>
                </li>
              ))}
            </ul>
            {qualityLevels[state].hasToOpen ? (
              <div className="flex flex-row items-center m-0 p-0">
                <button
                  className="bg-gray-200 hover:bg-[#38bdf8] py-2 px-4 mx-2 rounded-lg"
                  onClick={handleOpenMachine}
                >
                  Open machine
                </button>
                {isLoading ? (
                  <img
                    src={LoadingGif}
                    className="w-8 flex flex-row text-3xl pt-4"
                  />
                ) : (
                  ""
                )}
                {isComplete ? (
                  <p className="flex flex-row text-3xl pt-4 text-[#84cc16]">
                    <FaCheckCircle />
                  </p>
                ) : (
                  ""
                )}
              </div>
            ) : (
              ""
            )}
          </div>
        </div>
        <p
          className={`w-[32rem] ml-12 ${qualityLevels[state].color} p-4 my-2 rounded-xl text-white`}
        >
          {qualityLevels[state].warning}
        </p>
      </div>

      <Snackbar
        variant="soft"
        color="success"
        open={isComplete}
        onClose={() => {
          setOpen(false);
          setIsComplete(false);
        }}
        anchorOrigin={{ vertical: "top", horizontal: "right" }}
        endDecorator={
          <Button
            onClick={() => {
              setOpen(false);
              setIsComplete(false);
            }}
            size="sm"
            variant="soft"
            color="success"
          >
            Dismiss
          </Button>
        }
      >
        Your machine has been turned on.
      </Snackbar>
    </div>
  );
}

export default Detail;
