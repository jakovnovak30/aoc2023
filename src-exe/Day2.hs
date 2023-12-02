module Day2 where

import Data.List

solution :: [String] -> Int
-- solution = foldr (\(num, x) acc -> if x then acc + num else acc) 0 . map (\(x, y) -> (x, calculateBoolean y)) . map parseInput
solution = sum . map (calculatePower . snd) . map parseInput

parseInput :: String -> (Int, [String])
parseInput str = (read $ init $ head $ tail $ words str,
                  drop 2 $ words str)

calculateBoolean :: [String] -> Bool
calculateBoolean [] = True
calculateBoolean (a : b : xs)
  | read a > 12 && color b == "red" = False
  | read a > 13 && color b == "green" = False
  | read a > 14 && color b == "blue" = False
  | otherwise = calculateBoolean xs

calculatePower :: [String] -> Int
calculatePower = go 0 0 0
  where
    go :: Int -> Int -> Int -> [String] -> Int
    go minRed minBlue minGreen [] = minRed * minBlue * minGreen
    go minRed minBlue minGreen (a : b : ws)
      | color b == "red"   = go (max minRed (read a)) minBlue minGreen ws
      | color b == "green" = go minRed (max minBlue (read a)) minGreen ws
      | color b == "blue"  = go minRed minBlue (max minGreen (read a)) ws

color :: String -> String
color w = if not (w `elem` ["red", "green", "blue"])
            then init w
            else w
